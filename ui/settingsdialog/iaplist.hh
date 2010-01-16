#ifndef IAP_LIST_HH
#define IAP_LIST_HH

#include <QListWidget>

class Settings;

class IapList : public QListWidget {
  Q_OBJECT

public:
  IapList(Settings *settings, QWidget *parent = 0);
  ~IapList();

  QString selectedIap();

private slots:
  void iapAdded(const QString& id);
  void iapRemoved(const QString& id);
  void defaultIapSet(const QString& id);

private:
  Settings *m_settings;
};

#endif /* IAP_LIST_HH */
