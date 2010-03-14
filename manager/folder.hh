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
#define MESSAGE_STATUS_QUEUED 6

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
  Q_CLASSINFO("D-Bus Introspection", ""
	      "  <interface name=\"org.foolab.MMS.Folder\" >\n"
	      "    <signal name=\"messageAdded\" >\n"
	      "      <arg direction=\"out\" type=\"s\" name=\"id\" />\n"
	      "    </signal>\n"
	      "    <signal name=\"messageRemoved\" >\n"
	      "      <arg direction=\"out\" type=\"s\" name=\"id\" />\n"
	      "    </signal>\n"
	      "    <signal name=\"messageStatusChanged\" >\n"
	      "      <arg direction=\"out\" type=\"s\" name=\"id\" />\n"
	      "      <arg direction=\"out\" type=\"i\" name=\"status\" />\n"
	      "    </signal>\n"
	      "    <signal name=\"error\" >\n"
	      "      <arg direction=\"out\" type=\"s\" name=\"id\" />\n"
	      "      <arg direction=\"out\" type=\"s\" name=\"message\" />\n"
	      "    </signal>\n"
	      "    <method name=\"folderId\" >\n"
	      "      <arg direction=\"out\" type=\"s\" />\n"
	      "    </method>\n"
	      "    <method name=\"addMessage\" >\n"
	      "      <arg direction=\"out\" type=\"s\" />\n"
	      "      <arg direction=\"in\" type=\"ay\" name=\"data\" />\n"
	      "    </method>\n"
	      "    <method name=\"addMessage\" >\n"
	      "      <arg direction=\"out\" type=\"s\" />\n"
	      "      <arg direction=\"in\" type=\"s\" name=\"fullPath\" />\n"
	      "    </method>\n"
	      "    <method name=\"deleteMessage\" >\n"
	      "      <arg direction=\"out\" type=\"b\" />\n"
	      "      <arg direction=\"in\" type=\"s\" name=\"id\" />\n"
	      "    </method>\n"
	      "    <method name=\"allMessages\" >\n"
	      "      <arg direction=\"out\" type=\"as\" />\n"
	      "    </method>\n"
	      "    <method name=\"messageFileName\" >\n"
	      "      <arg direction=\"out\" type=\"s\" />\n"
	      "      <arg direction=\"in\" type=\"s\" name=\"id\" />\n"
	      "    </method>\n"
	      "    <method name=\"messageStatus\" >\n"
	      "      <arg direction=\"out\" type=\"i\" />\n"
	      "      <arg direction=\"in\" type=\"s\" name=\"id\" />\n"
	      "    </method>\n"
	      "    <method name=\"setMessageStatus\" >\n"
	      "      <arg direction=\"out\" type=\"b\" />\n"
	      "      <arg direction=\"in\" type=\"s\" name=\"id\" />\n"
	      "      <arg direction=\"in\" type=\"i\" name=\"status\" />\n"
	      "    </method>\n"
	      "  </interface>\n"
	      "")

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
