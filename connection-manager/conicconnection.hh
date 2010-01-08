#ifndef CONIC_CONNECTION_HH
#define CONIC_CONNECTION_HH

#include <QObject>

typedef struct _ConIcConnection ConIcConnection;

class ConicConnection : public QObject {
  Q_OBJECT

public:
  ConicConnection(const QString& name, QObject *parent = 0);
  ~ConicConnection();

  bool connect();
  void disconnect();

signals:
  void error(const QString& message);

private:
  const QString m_name;
  ConIcConnection *m_connection;
};

#endif /* CONIC_CONNECTION_HH */
