#include "iaplist.hh"
#include "settings.hh"

IapList::IapList(Settings *settings, QWidget *parent) : QListWidget(parent), m_settings(settings) {

  QObject::connect(m_settings, SIGNAL(iapAdded(const QString&)), this, SLOT(iapAdded(const QString&)));
  QObject::connect(m_settings, SIGNAL(iapRemoved(const QString&)), this, SLOT(iapRemoved(const QString&)));
  QObject::connect(m_settings, SIGNAL(defaultIapSet(const QString&)), this, SLOT(defaultIapSet(const QString&)));

  QStringList iaps = m_settings->allIaps();
  for (int x = 0; x < iaps.size(); x++) {
    iapAdded(iaps.at(x));
  }

  defaultIapSet(m_settings->defaultIap());
}

IapList::~IapList() {
  m_settings = 0;
}

QString IapList::selectedIap() {
  return currentItem() ? currentItem()->text() : QString();
}

void IapList::iapAdded(const QString& id) {
  QListWidgetItem *item = new QListWidgetItem(id, this);
  item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsUserCheckable);
  if (count() == 1) {
    setCurrentItem(item);
  }
}

void IapList::iapRemoved(const QString& id) {
  for (int x = 0; x < count(); x++) {
    if (item(x)->text() == id) {
      delete takeItem(x);
      return;
    }
  }
}

void IapList::defaultIapSet(const QString& id) {
  for (int x = 0; x < count(); x++) {
    item(x)->setCheckState(id == item(x)->text() ? Qt::Checked : Qt::Unchecked);
  }
  // TODO:
}
