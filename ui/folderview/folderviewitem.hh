#ifndef FOLDER_VIEW_ITEM_HH
#define FOLDER_VIEW_ITEM_HH

#include <QListWidgetItem>

class QFile;
class FolderViewItemFormatter;
class FolderView;

class FolderViewItem : public QListWidgetItem {
public:
  FolderViewItem(QFile *file, const QString& id, FolderViewItemFormatter *formatter, FolderView *parent = 0);
  ~FolderViewItem();

  QString id();

private:
  const QString m_id;
};

#endif /* FOLDER_VIEW_ITEM_HH */
