#include "connectionmanager.hh"
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>
#include <QDBusError>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  QDBusConnection c = QDBusConnection::systemBus();
  if (!c.registerService("org.foolab.MMS.ConnectionManager")) {
    qWarning() << "Failed to register org.foolab.MMS.ConnectionManager" << c.lastError().message();
    return 1;
  }

  ConnectionManager *mgr = new ConnectionManager(&app);
  Q_UNUSED(mgr);

  if (!c.registerObject("/org/foolab/mms/ConnectionManager", &app)) {
    qWarning() << "Failed to register /org/foolab/mms/ConnectionManager" << c.lastError().message();
    return 1;
  }

  return app.exec();
}
