#ifndef WAP_PUSH_HH
#define WAP_PUSH_HH

#include <QObject>
#include <QDBusAbstractAdaptor>

class WapPush : public QObject {
  Q_OBJECT

public:
  WapPush(QObject *parent = 0);
  ~WapPush();

signals:
  void addMessage(const QByteArray& message);

public slots:
  Q_NOREPLY void HandleWAPPush(const uchar& bearer, const QString& source, const int src, const int dst,
			       const QByteArray& header, const QByteArray& payload);

  Q_NOREPLY void HandleWAPPush(const uchar& bearer, const QByteArray& src, const QByteArray& dst,
			       const int sport, const int dport, const QByteArray& header, const QByteArray& payload);
};

class WapPushAdaptor : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.nokia.WAPPushHandler");

public:
  WapPushAdaptor(WapPush *parent);
  ~WapPushAdaptor();

public slots:
  Q_NOREPLY void HandleWAPPush(const uchar& bearer, const QString& source, const int src, const int dst,
			       const QByteArray& header, const QByteArray& payload);

  Q_NOREPLY void HandleWAPPush(const uchar& bearer, const QByteArray& src, const QByteArray& dst,
			       const int sport, const int dport, const QByteArray& header, const QByteArray& payload);

private:
  WapPush *m_push;
};

#endif /* WAP_PUSH_HH */
