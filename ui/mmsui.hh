#ifndef MMS_UI_HH
#define MMS_UI_HH

#include <QObject>
#include <QPointer>

class MmsWindow;

class MmsUi : public QObject {
  Q_OBJECT

public:
  MmsUi(QObject *parent = 0);
  ~MmsUi();

  void init();

private slots:
  void showSettings();
  void createNew();
  void showInbox();
  void showOutbox();
  void showReports();
  void showSent();
  void showIncoming();
  void showDrafts();

private:
  MmsWindow *setup(MmsWindow *window);

  QPointer<MmsWindow> m_main;
  QPointer<MmsWindow> m_incoming;
  QPointer<MmsWindow> m_outgoing;
  QPointer<MmsWindow> m_inbox;
  QPointer<MmsWindow> m_sent;
  QPointer<MmsWindow> m_reports;
  QPointer<MmsWindow> m_drafts;
};

#endif /* MMS_UI_HH */
