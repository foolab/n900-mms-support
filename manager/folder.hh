#ifndef FOLDER_HH
#define FOLDER_HH

#include <QObject>
#include <QStringList>
#include <QDBusAbstractAdaptor>

#define MESSAGE_STATUS_UNKNOWN 0
#define MESSAGE_STATUS_SENT 1
#define MESSAGE_STATUS_SENDING 2
#define MESSAGE_STATUS_FAILED 3
#define MESSAGE_STATUS_READ 4
#define MESSAGE_STATUS_UNREAD 5

class QSettings;

class Folder : public QObject {
  Q_OBJECT

public:
  Folder(const QString& id, QObject *parent = 0);
  virtual ~Folder();

  QString folderPath();

signals:
  void messageAdded(const QString& id);
  void messageRemoved(const QString& id);
  void messageStatusChanged(const QString& id, const int status);
  void error(const QString& id, const QString& message);

public slots:
  QString folderId();

  QString addMessage(const QByteArray& data);
  QString addMessage(const QString& fullPath);

  bool deleteMessage(const QString& id);

  QStringList allMessages();
  QString messageFileName(const QString& id);

  int messageStatus(const QString& id);

  bool setMessageStatus(const QString& id, const int& status);

private:
  void emitError(const QString& id, const QString& message);

  const QString m_id;
  const QString m_path;
  QSettings *m_meta;
};

class FolderAdaptor : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.foolab.MMS.Folder");

public:
  FolderAdaptor(Folder *parent);
  ~FolderAdaptor();

signals:
  void messageAdded(const QString& id);
  void messageRemoved(const QString& id);
  void messageStatusChanged(const QString& id, const int status);
  void error(const QString& id, const QString& message);

public slots:
  QString folderId();

  QString addMessage(const QByteArray& data);
  QString addMessage(const QString& fullPath);

  bool deleteMessage(const QString& id);

  QStringList allMessages();
  QString messageFileName(const QString& id);

  int messageStatus(const QString& id);

  bool setMessageStatus(const QString& id, const int& status);

private:
  Folder *m_folder;
};

#endif /* FOLDER_HH */
