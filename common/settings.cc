#include "settings.hh"
#include "gconf.hh"
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusError>
#include <QDebug>

#define DEFAULT_IAP_KEY QString("/apps/mms-support/default_iap")
#define IAP_ROOT QString("/system/osso/connectivity/IAP/")
#define AUTO_CONNECT_KEY ("/apps/mms-support/autoconnect")

Settings::Settings(QObject *parent) : QObject(parent) {
  m_imsi = 0;
  m_conf = new GConf;
}

Settings::~Settings() {
  delete m_conf;
  m_conf = 0;
}

QStringList Settings::allIaps() {
  QString root(IAP_ROOT);
  root.chop(1);
  QStringList res = m_conf->allDirs(root).replaceInStrings(IAP_ROOT, QString());

  QStringList ret;
  for (int x = 0; x < res.size(); x++) {
    if (m_conf->getBool(IAP_ROOT + res.at(x) + "/mms") == true) {
      ret.append(m_conf->unescape(res.at(x)));
    }
  }

  return ret;
}

void Settings::setDefaultIap(const QString& id) {
  m_conf->setString(DEFAULT_IAP_KEY, m_conf->escape(id));

  emit defaultIapSet(id);
}

QString Settings::defaultIap() {
  QStringList iaps = allIaps();
  QString iap = m_conf->unescape(m_conf->getString(DEFAULT_IAP_KEY));

  if (iap.isEmpty() || iaps.indexOf(iap) == -1) {
    if (iaps.size() > 1) {
      iap = iaps.at(0);
    }
    else {
      iap = QString();
    }

    setDefaultIap(iap);
  }

  return iap;
}

void Settings::removeIap(const QString& id) {
  Q_ASSERT(id.isEmpty() != true);

  if (id.isEmpty()) {
    return;
  }

  bool ret = m_conf->removeDir(IAP_ROOT + m_conf->escape(id));

  if (ret) {
    emit iapRemoved(id);
  }

  if (id == defaultIap()) {
    QStringList iaps = allIaps();
    if (iaps.size() > 1) {
      setDefaultIap(iaps.at(0));
    }
    else {
      setDefaultIap(QString());
    }
  }
}

bool Settings::autoConnect() {
  return m_conf->getBool(AUTO_CONNECT_KEY);
}

void Settings::setAutoConnect(bool enabled) {
  m_conf->setBool(AUTO_CONNECT_KEY, enabled);
}

bool Settings::addIap(const Iap& iap) {
  QString name = m_conf->escape(iap.name());
  QString path = IAP_ROOT + name;

  if (m_conf->dirExists(path)) {
    return false;
  }

  writeIap(iap);

  resetImsi(iap.name());

  emit iapAdded(iap.name());

  if (allIaps().size() == 1) {
    setDefaultIap(iap.name());
  }

  return true;
}

bool Settings::resetImsi(const QString& id) {
  QString name = m_conf->escape(id);
  QString path = IAP_ROOT + name;

  if (!m_conf->dirExists(path)) {
    return false;
  }

  if (!m_imsi) {
    m_imsi = new QDBusInterface("com.nokia.phone.SIM", "/com/nokia/phone/SIM", "Phone.Sim",
				QDBusConnection::systemBus(), this);
  }

  QDBusReply<QString> reply = m_imsi->call("get_imsi");
  if (!reply.isValid()) {
    qWarning() << "Failed to get IMSI" << reply.error().message();
    return false;
  }

  QString imsi = reply.value();
  if (imsi.isEmpty()) {
    qWarning() << "Got an empty IMSI";
    return false;
  }

  m_conf->setString(path + "/sim_imsi", imsi);

  return true;
}

Iap Settings::iap(const QString& id) {
  QString name = m_conf->escape(id);
  QString path = IAP_ROOT + name;

  Iap iap;
  iap.setName(id);

  iap.setApn(m_conf->getString(path + "/gprs_accesspointname"));
  iap.setPassword(m_conf->getString(path + "/gprs_password"));
  iap.setUsername(m_conf->getString(path + "/gprs_username"));
  iap.setProxyHost(m_conf->getString(path + "/mms_proxyhost"));
  iap.setMmscHost(m_conf->getString(path + "/mms_mmschost"));
  iap.setProxyPort(m_conf->getInt(path + "/mms_proxyport"));
  iap.setMmscPort(m_conf->getInt(path + "/mms_mmscport"));

  return iap;
}

bool Settings::saveIap(const Iap& iap) {
  QString name = m_conf->escape(iap.name());
  QString path = IAP_ROOT + name;

  if (!m_conf->dirExists(path)) {
    return false;
  }

  writeIap(iap);

  return true;
}

void Settings::writeIap(const Iap& iap) {
  QString name = m_conf->escape(iap.name());
  QString path = IAP_ROOT + name;

  m_conf->setString(path + "/type", "GPRS");
  m_conf->setString(path + "/name", iap.name());
  m_conf->setString(path + "/gprs_accesspointname", iap.apn());
  m_conf->setString(path + "/ipv4_type", "AUTO");
  m_conf->setBool(path + "/ask_password", false);
  m_conf->setString(path + "/gprs_password", iap.password());
  m_conf->setString(path + "/gprs_username", iap.username());
  m_conf->setString(path + "/mms_proxyhost", iap.proxyHost());
  m_conf->setString(path + "/mms_mmschost", iap.mmscHost());
  m_conf->setInt(path + "/mms_proxyport", iap.proxyPort());
  m_conf->setInt(path + "/mms_mmscport", iap.mmscPort());
  m_conf->setBool(path + "/mms", true);
}
