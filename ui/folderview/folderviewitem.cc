#include "folderviewitem.hh"
#include <QFile>
#include "mmsmessage.hh"
#include <QObject>
#include "folderview.hh"
#include "folderviewitemformatter.hh"

FolderViewItem::FolderViewItem(QFile *file, const QString& id, FolderViewItemFormatter *formatter, FolderView *parent) :
  QListWidgetItem(parent), m_id(id) {
  MmsMessage m(file);

  setText(formatter->format(m));
}

FolderViewItem::~FolderViewItem() {
}

QString FolderViewItem::id() {
  return m_id;
}
