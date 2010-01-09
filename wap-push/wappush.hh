#ifndef WAP_PUSH_HH
#define WAP_PUSH_HH

#include <QDBusAbstractAdaptor>

class WapPush : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.nokia.WAPPushHandler");

public:
  WapPush(QObject *parent = 0);
  ~WapPush();

public slots:
   Q_NOREPLY void HandleWAPPush(const uchar& bearer, const QString& source, const int src, const int dst,
				const QByteArray& header, const QByteArray& payload);
};

#endif /* WAP_PUSH_HH */
