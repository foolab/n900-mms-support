#ifndef IAP_HH
#define IAP_HH

#include <QString>

class Iap {
public:
  QString name() const;
  void setName(const QString& name);

  QString apn() const;
  void setApn(const QString& apn);

  QString username() const;
  void setUsername(const QString& username);

  QString password() const;
  void setPassword(const QString& password);

  QString proxyHost() const;
  void setProxyHost(const QString& proxyHost);

  quint32 proxyPort() const;
  void setProxyPort(quint32 proxyPort);

  QString mmscHost() const;
  void setMmscHost(const QString& mmscHost);

  quint32 mmscPort() const;
  void setMmscPort(quint32 mmscPort);

private:
  QString m_name;
  QString m_apn;
  QString m_username;
  QString m_password;
  QString m_proxyHost;
  quint32 m_proxyPort;
  QString m_mmscHost;
  quint32 m_mmscPort;
};

#endif /* IAP_HH */
