#include "iap.hh"

QString Iap::name() const {
  return m_name;
}

void Iap::setName(const QString& name) {
  m_name = name;
}

QString Iap::apn() const {
  return m_apn;
}

void Iap::setApn(const QString& apn) {
  m_apn = apn;
}

QString Iap::username() const {
  return m_username;
}

void Iap::setUsername(const QString& username) {
  m_username = username;
}

QString Iap::password() const {
  return m_password;
}

void Iap::setPassword(const QString& password) {
  m_password = password;
}

QString Iap::proxyHost() const {
  return m_proxyHost;
}

void Iap::setProxyHost(const QString& proxyHost) {
  m_proxyHost = proxyHost;
}

quint32 Iap::proxyPort() const {
  return m_proxyPort;
}

void Iap::setProxyPort(quint32 proxyPort) {
  m_proxyPort = proxyPort;
}

QString Iap::mmscHost() const {
  return m_mmscHost;
}

void Iap::setMmscHost(const QString& mmscHost) {
  m_mmscHost = mmscHost;
}

quint32 Iap::mmscPort() const {
  return m_mmscPort;
}

void Iap::setMmscPort(quint32 mmscPort) {
  m_mmscPort = mmscPort;
}

