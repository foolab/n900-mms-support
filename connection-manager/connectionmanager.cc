#include "connectionmanager.hh"
#include <gconf/gconf-client.h>
#include <QDebug>
#include <libosso.h>
#include "icd2listener.hh"
#include "conicconnection.hh"

#define IAP_NAME_PATH "/qpps/mms-support/iap"

ConnectionManager::ConnectionManager(QObject *parent) : QDBusAbstractAdaptor(parent) {
  setAutoRelaySignals(true);
  m_connected = false;
  m_connection = 0;

  g_type_init();

  m_context = osso_initialize("org.foolab.MMS.ConnectionManager", "0.1", TRUE, NULL);

  m_client = gconf_client_get_default();

  m_listener = new Icd2Listener(this);
  QObject::connect(m_listener, SIGNAL(connected(const QString&)), this, SLOT(icdConnected(const QString&)));
  QObject::connect(m_listener, SIGNAL(disconnected(const QString&)), this, SLOT(icdDisconnected(const QString&)));
}

ConnectionManager::~ConnectionManager() {
  if (m_connected) {
    disconnect();
  }

  osso_deinitialize(m_context);
  g_object_unref(m_client);
}

QString ConnectionManager::name() {
  char *name = gconf_client_get_string(m_client, IAP_NAME_PATH, NULL);
  if (!name) {
    return QString();
  }

  QString iap = name;

  g_free(name);

  return iap;
}

bool ConnectionManager::connect() {
  if (m_connected) {
    emit connected();
    return true;
  }

  QString iap = name();

  if (iap.isEmpty()) {
    QString message = "No IAP defined.";
    qWarning() << "ConnectionManager::connect" << message;
    emit error(message);
    return false;
  }

  Q_ASSERT(m_connection == 0);

  m_connection = new ConicConnection(iap, this);
  QObject::connect(m_connection, SIGNAL(error(const QString&)), this, SIGNAL(conicError(const QString&)));

  return m_connection->connect();
}

void ConnectionManager::disconnect() {
  if (!m_connected) {
    return;
  }

  m_connection->disconnect();
  m_connection->deleteLater();
  m_connection = 0;
}

void ConnectionManager::icdConnected(const QString& iap) {
  if (name() == iap) {
    m_connected = true;
    emit connected();
  }
}

void ConnectionManager::icdDisconnected(const QString& iap) {
  if (name() == iap) {
    m_connected = false;
    emit disconnected();

    m_connection->deleteLater();
    m_connection = 0;
  }
}

void ConnectionManager::conicError(const QString& message) {
  qWarning() << "ConnectionManager::conicError" << message;
  emit error(message);
  m_connection->deleteLater();
  m_connection = 0;
}

//#define IAP_PATH "/system/osso/connectivity/IAP/"
#if 0
QString ConnectionManager::name() {
  char *name = gconf_client_get_string(m_client, IAP_NAME_PATH, NULL);
  if (!name) {
    return QString();
  }

  QString iap = name;

  g_free(name);

  return iap;
}

bool ConnectionManager::setName(const QString& name) {
  // TODO:
  /////  return setIapEntry("gprs_accesspointname", name);
}

QString ConnectionManager::apn() {
  return iapEntry("gprs_accesspointname");
}

bool ConnectionManager::setApn(const QString& apn) {
  return setIapEntry("gprs_accesspointname", apn);
}

QString ConnectionManager::username() {
  return iapEntry("gprs_username");
}

bool ConnectionManager::setUsername(const QString& username) {
  return setIapEntry("gprs_username", username);
}

QString ConnectionManager::password() {
  return iapEntry("gprs_password");
}

bool ConnectionManager::setPassword(const QString& password) {
  return setIapEntry("gprs_password", password);
}

QString ConnectionManager::imsi() {
  // TODO:
}

bool ConnectionManager::isConnected() {
  // TODO:
}

QString ConnectionManager::iapEntry(const QString& var) {
  QString n = name();
  if (n.isEmpty()) {
    return n;
  }

  QString path = QString("%1%2/%3").arg(IAP_PATH).arg(n).arg(var);

  char *v = gconf_client_get_string(m_client, path.toUtf8().data(), NULL);
  if (!v) {
    return QString();
  }

  QString ret = v;

  g_free(v);

  return ret;
}

bool ConnectionManager::setIapEntry(const QString& var, const QString& val) {
  QString n = name();
  if (n.isEmpty()) {
    return false;
  }

  QString path = QString("%1%2/%3").arg(IAP_PATH).arg(n).arg(var);

  return gconf_client_set_string(m_client, path.toUtf8().data(), val.toUtf8().data(), NULL) == TRUE;
}

/*
 dbus-send --system --print-reply --type=method_call --dest=com.nokia.phone.SIM /com/nokia/phone/SIM Phone.Sim.get_imsi

dbus-send --system --print-reply --type=method_call --dest=com.nokia.phone.SIM /com/nokia/phone/SIM/security Phone.Sim.Security.get_imei
*/
#endif
