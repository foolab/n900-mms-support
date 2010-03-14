#include "wappush.hh"
#include <QDebug>

WapPush::WapPush(QObject *parent) : QObject(parent) {
  WapPushAdaptor *adaptor = new WapPushAdaptor(this);
  Q_UNUSED(adaptor);
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

  qDebug() << "WapPush::HandleWAPPush";

  emit addMessage(payload);
}

void WapPush::HandleWAPPush(const uchar& bearer, const QByteArray& src, const QByteArray& dst,
			    const int sport, const int dport, const QByteArray& header, const QByteArray& payload) {

  Q_UNUSED(bearer);
  Q_UNUSED(src);
  Q_UNUSED(dst);
  Q_UNUSED(sport);
  Q_UNUSED(dport);
  Q_UNUSED(header);
  Q_UNUSED(payload);

  qDebug() << "WapPush::HandleWAPPush2";
}



WapPushAdaptor::WapPushAdaptor(WapPush *parent) : QDBusAbstractAdaptor(parent), m_push(parent) {

}

WapPushAdaptor::~WapPushAdaptor() {

}

void WapPushAdaptor::HandleWAPPush(const uchar& bearer, const QString& source, const int src, const int dst,
				   const QByteArray& header, const QByteArray& payload) {
  m_push->HandleWAPPush(bearer, source, src, dst, header, payload);
}

void WapPushAdaptor::HandleWAPPush(const uchar& bearer, const QByteArray& src, const QByteArray& dst,
				   const int sport, const int dport, const QByteArray& header, const QByteArray& payload) {
  m_push->HandleWAPPush(bearer, src, dst, sport, dport, header, payload);
}
