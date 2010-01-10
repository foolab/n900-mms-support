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

template <typename T> T *createAndRegisterFolder(QDBusConnection& c, const QString& id) {
  T *t = new T(qApp);

  QString objectPath = QString("/org/foolab/MMS/Manager/%1").arg(id);
  if (!c.registerObject(objectPath, t)) {
    qWarning() << "Failed to register" << objectPath << c.lastError().message();
    return 0;
  }

  return t;
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

  Incoming *i = createAndRegisterFolder<Incoming>(c, "Incoming");
  if (!i) {
    return 1;
  }

  Folder *f = 0;
  f = createAndRegisterFolder<Sent>(c, "Sent");
  if (!f) {
    return 1;
  }

  f = createAndRegisterFolder<Inbox>(c, "Inbox");
  if (!f) {
    return 1;
  }

  f = createAndRegisterFolder<Outgoing>(c, "Outgoing");
  if (!f) {
    return 1;
  }

  f = createAndRegisterFolder<Reports>(c, "Reports");
  if (!f) {
    return 1;
  }

  QObject::connect(p, SIGNAL(addMessage(const QByteArray&)), i, SLOT(addMessage(const QByteArray&)));

  return app.exec();
}
