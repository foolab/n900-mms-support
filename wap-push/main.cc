#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>
#include <QDBusError>
#include "wappush.hh"

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  QDBusConnection c = QDBusConnection::systemBus();
  if (!c.registerService("org.foolab.MMS.WapPush")) {
    qWarning() << "Failed to register org.foolab.MMS.WapPush" << c.lastError().message();
    return 1;
  }

  WapPush *p = new WapPush(&app);
  Q_UNUSED(p);

  if (!c.registerObject("/org/foolab/MMS/WapPush", &app)) {
    qWarning() << "Failed to register /org/foolab/MMS/WapPush" << c.lastError().message();
    return 1;
  }

  return app.exec();
}
