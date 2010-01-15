#ifndef SETTINGS_HH
#define SETTINGS_HH

#include <QObject>
#include <QStringList>
#include "iap.hh"

class GConf;
class QDBusInterface;

class Settings : public QObject {
  Q_OBJECT

public:
  Settings(QObject *parent = 0);
  ~Settings();

  QStringList allIaps();

  void setDefaultIap(const QString& id);

  QString defaultIap();

  Iap iap(const QString& id);
  bool addIap(const Iap& iap);
  bool saveIap(const Iap& iap);
  void removeIap(const QString& id);

  bool autoConnect();
  void setAutoConnect(bool enabled);

public slots:
  bool resetImsi(const QString& id);

signals:
  void iapAdded(const QString& iap);
  void iapRemoved(const QString& iap);
  void defaultIapSet(const QString& iap);

private:
  void writeIap(const Iap& iap);

  GConf *m_conf;
  QDBusInterface *m_imsi;
};

#endif /* SETTINGS_HH */
