#ifndef ADAPTOR_HH
#define ADAPTOR_HH

#include <QDBusAbstractAdaptor>
#include <QStringList>

class Folder;

class Adaptor : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.foolab.MMS.Folder");

public:
  Adaptor(Folder *parent);
  ~Adaptor();

signals:
  void messageAdded(const QString& id);
  void messageRemoved(const QString& id);

public slots:
  QString addMessage(const QByteArray& data);
  bool deleteMessage(const QString& id);
  QStringList allMessages();
  QString messageFileName(const QString& id);

private:
  Folder *m_folder;
};

#endif /* ADAPTOR_HH */
