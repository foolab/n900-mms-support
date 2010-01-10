#ifndef FOLDER_HH
#define FOLDER_HH

#include <QObject>
#include <QStringList>

class Folder : public QObject {
  Q_OBJECT

public:
  Folder(const QString& id, QObject *parent = 0);
  virtual ~Folder();

  QString folderId();
  QString folderPath();

signals:
  void messageAdded(const QString& id);
  void messageRemoved(const QString& id);

public slots:
  QString addMessage(const QByteArray& data);
  bool deleteMessage(const QString& id);

  QStringList allMessages();
  QString messageFileName(const QString& id);

private:
  const QString m_id;
  const QString m_path;
};

#endif /* FOLDER_HH */
