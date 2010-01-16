#include "folderview.hh"
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>
#include "folderviewitem.hh"
#include <QFile>

FolderView::FolderView(const QString& id, FolderViewItemFormatter *formatter, QWidget *parent) :
  QListWidget(parent), m_formatter(formatter) {
  QString path = QString("/org/foolab/MMS/Manager/%1").arg(id);
  m_interface = new QDBusInterface("org.foolab.MMS.Manager", path,
				   "org.foolab.MMS.Manager.Folder", QDBusConnection::systemBus(), this);

  QObject::connect(m_interface, SIGNAL(messageAdded(const QString&)), this, SLOT(messageAdded(const QString&)));
  QObject::connect(m_interface, SIGNAL(messageRemoved(const QString&)), this, SLOT(messageRemoved(const QString&)));

  QDBusReply<QStringList> reply = m_interface->call("allMessages");
  QStringList list = reply.value();
   for (int x = 0; x < list.size(); x++) {
    messageAdded(list.at(x));
  }
}

FolderView::~FolderView() {

}

void FolderView::messageAdded(const QString& id) {
  QDBusReply<QString> r = m_interface->call("messageFileName", id);

  QString path = r.value();
  QFile file(path, this);
  if (!file.open(QFile::ReadOnly)) {
    // TODO: error.
    return;
  }

  FolderViewItem *item = new FolderViewItem(&file, id, m_formatter, this);

  Q_UNUSED(item);
}

void FolderView::messageRemoved(const QString& id) {
  for (int x = 0; x < count(); x++) {
    FolderViewItem *i = dynamic_cast<FolderViewItem *>(item(x));
    if (i && i->id() == id) {
      delete takeItem(x);
      // TODO: cancel download ?
      return;
    }
  }
}

QString FolderView::selectedId() {
  FolderViewItem *i = dynamic_cast<FolderViewItem *>(currentItem());
  if (i) {
      return i->id();
  }

  return QString();
}

void FolderView::deleteMessage(const QString& id) {
  m_interface->call("deleteMessage", id);
}
