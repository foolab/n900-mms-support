#ifndef MAIN_WINDOW_HH
#define MAIN_WINDOW_HH

#include "mmswindow.hh"

class QListWidgetItem;

class MainWindow : public MmsWindow {
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow();

signals:
  void showInbox();
  void showOutbox();
  void showReports();
  void showSent();
  void showIncoming();
  void showDrafts();

protected:
  void doCreateContent();

private slots:
  void itemClicked(QListWidgetItem *item);

private:
  enum SubWindow {
    InboxWindow,
    OutboxWindow,
    SentWindow,
    ReportsWindow,
    IncomingWindow,
    DraftsWindow,
  };
};

#endif /* MAIN_WINDOW_HH */
