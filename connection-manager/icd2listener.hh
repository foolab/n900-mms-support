#ifndef ICD2_LISTENER_HH
#define ICD2_LISTENER_HH

#include <QDBusInterface>

class Icd2Listener : public QDBusInterface {
  Q_OBJECT

public:
  Icd2Listener(QObject *parent = 0);

signals:
  void connected(const QString& iap);
  void disconnected(const QString& iap);

private slots:
  void stateSig(const QDBusMessage& message);
  void requestState();
};

#endif /* ICD2_LISTENER_HH */
