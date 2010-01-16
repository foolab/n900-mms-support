#ifndef INCOMING_HH
#define INCOMING_HH

#include "mmswindow.hh"

class QAction;
class FolderView;
class FolderViewItemFormatter;

class Incoming : public MmsWindow {
  Q_OBJECT

public:
  Incoming(MmsWindow *parent = 0);
  ~Incoming();

protected:
  void doCreateContent();

private slots:
  void deleteSelected();

private:
  QAction *m_download;
  QAction *m_delete;
  QAction *m_remote;
  FolderView *m_view;
  FolderViewItemFormatter *m_formatter;
};

#endif /* INCOMING_HH */
