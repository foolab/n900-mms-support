#include "folder.hh"
#include <QDir>
#include <QDebug>
#include <QTemporaryFile>
#include "adaptor.hh"

Folder::Folder(const QString& id, QObject *parent) :
  QObject(parent), m_id(id), m_path(QString("/home/user/.mms/%1/").arg(id)) {

  Adaptor *a = new Adaptor(this);
  Q_UNUSED(a);

  QDir d(m_path);
  if (!d.exists()) {
    if (!d.mkpath(".")) {
      qWarning() << "Failed to create" << m_path;
    }
  }
}

Folder::~Folder() {

}

QString Folder::folderId() {
  return m_id;
}

QString Folder::folderPath() {
  return m_path;
}

QString Folder::addMessage(const QByteArray& data) {
  QTemporaryFile f(messageFileName("XXXXXX"));

  f.setAutoRemove(false);
  if (!f.open()) {
    qWarning() << "Failed to create a file" << f.errorString();
    return QString();
  }

  if (f.write(data) != data.size()) {
    qWarning() << "Failed to write to file" << f.fileName() << f.errorString();
    return QString();
  }

  QFileInfo i(f.fileName());

  f.close();

  qDebug() << "added message" << i.fileName();

  emit messageAdded(i.fileName());

  return i.fileName();
}

bool Folder::deleteMessage(const QString& id) {
  QFile f(messageFileName(id));

  if (!f.exists()) {
    qWarning() << "Message" << id << "does not exist.";

    emit messageRemoved(id);

    return true;
  }

  if (!f.remove()) {
    qWarning() << "Failed to remove" << id << f.errorString();
    return false;
  }

  emit messageRemoved(id);

  return true;
}

QStringList Folder::allMessages() {
  QString lol("lll");

  emit messageAdded(lol);

  return QDir(m_path).entryList(QDir::Files);
}

QString Folder::messageFileName(const QString& id) {
  return m_path + "/" + id;
}
