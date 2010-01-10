#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>
#include <QDBusError>
#include "folders.hh"
#include "wappush.hh"

WapPush *createAndRegisterWapPush(QDBusConnection& c) {
  if (!c.registerService("org.foolab.MMS.WapPush")) {
    qWarning() << "Failed to register org.foolab.MMS.WapPush" << c.lastError().message();
    return 0;
  }

  WapPush *p = new WapPush(qApp);

  if (!c.registerObject("/org/foolab/MMS/WapPush", p)) {
    qWarning() << "Failed to register /org/foolab/MMS/WapPush" << c.lastError().message();
    return 0;
  }

  return p;
}


Incoming *createAndRegisterIncoming(QDBusConnection& c) {
  Incoming *i = new Incoming(qApp);

  if (!c.registerObject("/org/foolab/MMS/Manager/Incoming", i)) {
    qWarning() << "Failed to register /org/foolab/MMS/Manager/Incoming" << c.lastError().message();
    return 0;
  }

  return i;
}

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  QDBusConnection c = QDBusConnection::systemBus();

  if (!c.registerService("org.foolab.MMS.Manager")) {
    qWarning() << "Failed to register org.foolab.MMS.Manager" << c.lastError().message();
    return 1;
  }

  WapPush *p = createAndRegisterWapPush(c);
  if (!p) {
    return 1;
  }

  Incoming *i = createAndRegisterIncoming(c);
  if (!i) {
    return 1;
  }

  QObject::connect(p, SIGNAL(addMessage(const QByteArray&)), i, SLOT(addMessage(const QByteArray&)));

  return app.exec();
}
