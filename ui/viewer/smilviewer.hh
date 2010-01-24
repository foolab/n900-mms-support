#ifndef SMIL_VIEWER_HH
#define SMIL_VIEWER_HH

#include <QMainWindow>

class MmsMessage;
class MmsPart;
class QGridLayout;

class SmilViewer : public QMainWindow {
  Q_OBJECT

public:
  SmilViewer(const QString& path, QMainWindow *parent = 0);
  ~SmilViewer();

private:
  void displayAll(const QList<MmsPart>& parts);

  QWidget *imageWidget(const MmsPart& part);
  QWidget *textWidget(const MmsPart& part);
  QWidget *audioWidget(const MmsPart& part);
  QWidget *videoWidget(const MmsPart& part);
  QWidget *unknownWidget(const MmsPart& part);

  QGridLayout *m_layout;
  MmsMessage *m_message;
};

#endif /* SMIL_VIEWER_HH */
