#include "folder.hh"
#include <QDir>
#include <QDebug>
#include <QTemporaryFile>
#include <QSettings>

Folder::Folder(const QString& id, QObject *parent) :
  QObject(parent), m_id(id), m_path(QString("/home/user/.mms/%1").arg(id)) {

  FolderAdaptor *a = new FolderAdaptor(this);
  Q_UNUSED(a);

  QDir d(m_path + "/");
  if (!d.exists()) {
    if (!d.mkpath(".")) {
      qWarning() << "Failed to create" << m_path;
    }
  }

  m_meta = new QSettings(m_path + ".meta", QSettings::IniFormat, this);
}

Folder::~Folder() {

}

QString Folder::folderPath() {
  return m_path + "/";
}

QString Folder::folderId() {
  return m_id;
}

QString Folder::addMessage(const QByteArray& data) {
  QTemporaryFile f(messageFileName("XXXXXX"));

  f.setAutoRemove(false);
  if (!f.open()) {
    emitError(QString(), tr("Failed to create a file: %1").arg(f.errorString()));
    return QString();
  }

  if (f.write(data) != data.size()) {
    emitError(QString(), tr("Failed to write to file %1: %2").arg(f.fileName()).arg(f.errorString()));
    f.remove();
    return QString();
  }

  QFileInfo i(f.fileName());

  f.close();

  //  qDebug() << "added message" << i.fileName();

  setMessageStatus(i.fileName(), MESSAGE_STATUS_UNREAD);

  emit messageAdded(i.fileName());

  return i.fileName();
}

QString Folder::addMessage(const QString& fullPath) {
  QFile f(fullPath);
  if (!f.open(QFile::ReadOnly)) {
    emitError(QString(), tr("Failed to open file %1: %2").arg(f.fileName()).arg(f.errorString()));
    return false;
  }

  QByteArray data = f.readAll();
  if (f.error() != QFile::NoError) {
    emitError(QString(), tr("Failed to read from file %1: %2").arg(f.fileName()).arg(f.errorString()));
    return false;
  }

  return addMessage(data);
}

bool Folder::deleteMessage(const QString& id) {
  QFile f(messageFileName(id));

  if (!f.exists()) {
    emitError(id, tr("Message does not exist"));

    m_meta->remove(id);
    emit messageRemoved(id);

    return true;
  }

  if (!f.remove()) {
    emitError(QString(), tr("Failed to remove file %1: %2").arg(f.fileName()).arg(f.errorString()));
    return false;
  }

  m_meta->remove(id);
  emit messageRemoved(id);

  return true;
}

QStringList Folder::allMessages() {
  return QDir(m_path).entryList(QDir::Files);
}

QString Folder::messageFileName(const QString& id) {
  return m_path + "/" + id;
}

int Folder::messageStatus(const QString& id) {
  return m_meta->value(id + "/status").toInt();
}

bool Folder::setMessageStatus(const QString& id, const int& status) {
  if (allMessages().indexOf(id) == -1) {
    emitError(id, tr("Unknown message"));
    return false;
  }

  if (status != messageStatus(id)) {
    m_meta->setValue(id + "/status", status);
    emit messageStatusChanged(id, status);
  }

  return true;
}

void Folder::emitError(const QString& id, const QString& message) {
  QString er = QString("Folder %1: ").arg(folderId());
  if (!id.isEmpty()) {
    er.append(QString("(Message %1) ").arg(id));
  }

  er.append(message);

  qWarning() << er;

  emit error(id, message);
}

FolderAdaptor::FolderAdaptor(Folder *parent) : QDBusAbstractAdaptor(parent), m_folder(parent) {
  setAutoRelaySignals(true);
}

FolderAdaptor::~FolderAdaptor() {

}

QString FolderAdaptor::folderId() {
  return m_folder->folderId();
}

QString FolderAdaptor::addMessage(const QByteArray& data) {
  return m_folder->addMessage(data);
}

QString FolderAdaptor::addMessage(const QString& fullPath) {
  return m_folder->addMessage(fullPath);
}

bool FolderAdaptor::deleteMessage(const QString& id) {
  return m_folder->deleteMessage(id);
}

QStringList FolderAdaptor::allMessages() {
  return m_folder->allMessages();
}

QString FolderAdaptor::messageFileName(const QString& id) {
  return m_folder->messageFileName(id);
}

int FolderAdaptor::messageStatus(const QString& id) {
  return m_folder->messageStatus(id);
}

bool FolderAdaptor::setMessageStatus(const QString& id, const int& status) {
  return m_folder->setMessageStatus(id, status);
}
