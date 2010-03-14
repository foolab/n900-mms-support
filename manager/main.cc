#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>
#include <QDBusError>
#include "folders.hh"
#include "wappush.hh"
#include "queuemanager.hh"
#include "mmsprotocolhandler.hh"

WapPush *createAndRegisterWapPush(QDBusConnection& c) {
  // TODO: remove this extra service.
  if (!c.registerService("org.foolab.MMS.WapPush")) {
    qWarning() << "Failed to register org.foolab.MMS.WapPush" << c.lastError().message();
    return 0;
  }

  WapPush *p = new WapPush(qApp);

  if (!c.registerObject("/org/foolab/MMS/WapPush", p)) {
    qWarning() << "Failed to register /org/foolab/MMS/WapPush" << c.lastError().message();
    return 0;
  }

  return p;
}

template <typename T> T *createAndRegisterFolder(QDBusConnection& c, const QString& id) {
  T *t = new T(qApp);

  QString objectPath = QString("/org/foolab/MMS/Folders/%1").arg(id);
  if (!c.registerObject(objectPath, t)) {
    qWarning() << "Failed to register" << objectPath << c.lastError().message();
    return 0;
  }

  return t;
}

QueueManager *createAndRegisterManager(QDBusConnection& c) {
  if (!c.registerService("org.foolab.MMS.QueueManager")) {
    qWarning() << "Failed to register org.foolab.MMS.QueueManager" << c.lastError().message();
    return 0;
  }

  QueueManager *manager = new QueueManager(qApp);
  if (!c.registerObject("/org/foolab/MMS/QueueManager", manager)) {
    qWarning() << "Failed to register /org/foolab/MMS/QueueManager" << c.lastError().message();
    manager->deleteLater();
    return 0;
  }

  return manager;
}

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  QDBusConnection c = QDBusConnection::systemBus();

  WapPush *pushHandler = createAndRegisterWapPush(c);

  Incoming *incoming = createAndRegisterFolder<Incoming>(c, "Incoming");
  Outbox *outbox = createAndRegisterFolder<Outbox>(c, "Outbox");
  Inbox *inbox = createAndRegisterFolder<Inbox>(c, "Inbox");
  Sent *sent = createAndRegisterFolder<Sent>(c, "Sent");
  Reports *reports = createAndRegisterFolder<Reports>(c, "Reports");
  Failed *failed = createAndRegisterFolder<Failed>(c, "Failed");
  Pushed *pushed = createAndRegisterFolder<Pushed>(c, "Pushed");
  Queue *queue = createAndRegisterFolder<Queue>(c, "Queue");





  QueueManager *manager = createAndRegisterManager(c);

  MmsProtocolHandler *handler = new MmsProtocolHandler(qApp);

  if (!pushHandler || !incoming || !outbox || !inbox || !sent || !reports || !failed || !pushed || !queue || !manager) {
    return 1;
  }

  QObject::connect(pushHandler, SIGNAL(addMessage(const QByteArray&)), pushed, SLOT(addMessage(const QByteArray&)));
  handler->setPushed(pushed);
  handler->setIncoming(incoming);
  handler->setReports(reports);

  /*
   * Handler will move NotificationInd from Pushed to Incoming.
   * The user asks to download an MMS so it gets moved to Queue.
   * QueueManager picks it up and downloads it.
   *
   * QueueManager will move messages to either Incoming or Inbox.
   */

  //  handler->setOutbox(outbox);
  QObject::connect(manager, SIGNAL(messageDownloaded(const QString&, const QString&)),
		   handler, SLOT(messageDownloaded(const QString&, const QString&)));

  manager->setQueue(queue);
  manager->setIncoming(incoming);
  manager->setInbox(inbox);

  //  MmsSender *sender
//  Sender *sender = new Sender(this);
//  sender->setOutbox(outbox);

  return app.exec();
}
