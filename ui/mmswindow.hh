#ifndef MMS_WINDOW_HH
#define MMS_WINDOW_HH

#include <QMainWindow>

class MmsWindow : public QMainWindow {
  Q_OBJECT

public:
  MmsWindow(MmsWindow *parent = 0);
  virtual ~MmsWindow();

  void createContent();

signals:
  void createNew();
  void showSettings();

protected:
  virtual void doCreateContent() = 0;

private:
  bool m_created;
};

#endif /* MMS_WINDOW_HH */
