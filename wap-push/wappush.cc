#include "wappush.hh"
#include <QDebug>
#include <QDBusInterface>
#include <QDBusConnection>

WapPush::WapPush(QObject *parent) : QDBusAbstractAdaptor(parent) {

}

WapPush::~WapPush() {

}

void WapPush::HandleWAPPush(const uchar& bearer, const QString& source, const int src, const int dst,
			    const QByteArray& header, const QByteArray& payload) {

  Q_UNUSED(bearer);
  Q_UNUSED(source);
  Q_UNUSED(src);
  Q_UNUSED(dst);
  Q_UNUSED(header);

  QDBusInterface i("org.foolab.MMS.Manager", "/org/foolab/MMS/Manager/Incoming", "org.foolab.MMS.Manager.Incoming",
		   QDBusConnection::systemBus());

  i.call("add", payload);
}
