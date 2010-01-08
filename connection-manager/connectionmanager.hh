#ifndef CONNECTION_MANAGER_HH
#define CONNECTION_MANAGER_HH

#include <QDBusAbstractAdaptor>

typedef struct _GConfClient GConfClient;
typedef struct osso_af_context_t osso_context_t;
class Icd2Listener;
class ConicConnection;

class ConnectionManager : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.foolab.MMS.ConnectionManager");
  Q_PROPERTY(QString name READ name);

    //  Q_PROPERTY(QString apn READ apn WRITE setApn)
    //  Q_PROPERTY(QString username READ username WRITE setUsername)
    //  Q_PROPERTY(QString password READ password WRITE setPassword)
    //  Q_PROPERTY(QString imsi READ imsi)
    //  Q_PROPERTY(QString name READ name WRITE setName)

    /*
  Q_PROPERTY(QString proxyHost READ proxyHost WRITE setProxyHost)
  Q_PROPERTY(quint32 proxyPort READ proxyPort WRITE setProxyPort)
  Q_PROPERTY(QString mmscHost READ mmscHost WRITE setMmscHost)
  Q_PROPERTY(quint32 mmscPort READ mmscPort WRITE setMmscPort)

  Q_PROPERTY()
    */
public:
  ConnectionManager(QObject *parent = 0);
  ~ConnectionManager();
  /*
  QString apn();
  bool setApn(const QString& apn);

  QString username();
  bool setUsername(const QString& username);

  QString password();
  bool setPassword(const QString& password);

  QString imsi();
  */

  /*
  QString proxyHost();
  void setProxyHost(const QString& host);

  quint32 proxyPort();
  void setProxyPort(const quint32& port);

  QString mmscHost();
  void setMmscHost(const QString& host);

  quint32 mmscPort();
  void setMmscPort(const quint32& port);
  */
  QString name();
  //  bool setName(const QString& name);

public slots:
  bool connect();
  Q_NOREPLY void disconnect();

signals:
  void connected();
  void disconnected();
  void error(const QString& message);

private slots:
  void icdConnected(const QString& iap);
  void icdDisconnected(const QString& iap);
  void conicError(const QString& message);

private:
  //  QString iapEntry(const QString& var);
  //  bool setIapEntry(const QString& var, const QString& val);
  bool m_connected;
  osso_context_t *m_context;
  GConfClient *m_client;
  Icd2Listener *m_listener;

  ConicConnection *m_connection;
};

#endif /* CONNECTION_MANAGER_HH */
