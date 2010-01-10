#include "adaptor.hh"
#include "folder.hh"

Adaptor::Adaptor(Folder *parent) : QDBusAbstractAdaptor(parent), m_folder(parent) {
  setAutoRelaySignals(true);
}

Adaptor::~Adaptor() {

}

QString Adaptor::addMessage(const QByteArray& data) {
  return m_folder->addMessage(data);
}

bool Adaptor::deleteMessage(const QString& id) {
  return m_folder->deleteMessage(id);
}

QStringList Adaptor::allMessages() {
  return m_folder->allMessages();
}

QString Adaptor::messageFileName(const QString& id) {
  return m_folder->messageFileName(id);
}
