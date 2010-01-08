#include "conicconnection.hh"
//#include <coniciap.h>
#include <conicconnection.h>

ConicConnection::ConicConnection(const QString& name, QObject *parent) : QObject(parent), m_name(name), m_connection(0) {

}

ConicConnection::~ConicConnection() {
  disconnect();
}

bool ConicConnection::connect() {
  Q_ASSERT(m_connection == 0);
  m_connection = con_ic_connection_new();
  return con_ic_connection_connect_by_id(m_connection, m_name.toUtf8().data(), CON_IC_CONNECT_FLAG_NONE) == TRUE;
}

void ConicConnection::disconnect() {
  if (m_connection) {
    con_ic_connection_disconnect_by_id(m_connection, m_name.toUtf8().data());
    g_object_unref(m_connection);
    m_connection = 0;
  }
}
