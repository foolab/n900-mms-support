#include "icd2listener.hh"
#include <icd/dbus_api.h>
#include <QTimer>

Icd2Listener::Icd2Listener(QObject *parent)
  : QDBusInterface(ICD_DBUS_API_INTERFACE, ICD_DBUS_API_PATH, ICD_DBUS_API_INTERFACE,
		   QDBusConnection::systemBus(), parent) {

  connection().connect(ICD_DBUS_API_INTERFACE, ICD_DBUS_API_PATH, ICD_DBUS_API_INTERFACE,
		       ICD_DBUS_API_STATE_SIG, this, SLOT(stateSig(const QDBusMessage&)));

  QTimer::singleShot(0, this, SLOT(requestState()));
}

void Icd2Listener::requestState() {
  call(ICD_DBUS_API_STATE_REQ);
}

void Icd2Listener::stateSig(const QDBusMessage& message) {
  QList<QVariant> args = message.arguments();
  if (args.size() < 8) {
    return;
  }

  quint32 st = args.at(7).toInt();
  QString id = QString::fromUtf8(args.at(5).toByteArray());

  if (st == ICD_STATE_CONNECTED) {
    emit connected(id);
  }
  else if (st == ICD_STATE_DISCONNECTED) {
    emit disconnected(id);
  }
}
