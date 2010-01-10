#include "wappush.hh"
#include <QDebug>

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

  emit addMessage(payload);
}
