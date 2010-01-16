#ifndef FOLDER_VIEW_HH
#define FOLDER_VIEW_HH

#include <QListWidget>

class QDBusInterface;
class FolderViewItemFormatter;

class FolderView : public QListWidget {
  Q_OBJECT

public:
  FolderView(const QString& id, FolderViewItemFormatter *formatter, QWidget *parent = 0);
  ~FolderView();

  QString selectedId();

public slots:
  void deleteMessage(const QString& id);

private slots:
  void messageAdded(const QString& id);
  void messageRemoved(const QString& id);

private:
  FolderViewItemFormatter *m_formatter;
  QDBusInterface *m_interface;
};

#endif /* FOLDER_VIEW_HH */
