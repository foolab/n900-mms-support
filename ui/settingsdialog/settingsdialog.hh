#ifndef SETTINGS_DIALOG_HH
#define SETTINGS_DIALOG_HH

#include <QDialog>

class QCheckBox;
class IapList;
class Settings;

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  SettingsDialog(QWidget *parent = 0);
  ~SettingsDialog();

private slots:
  void addIap();
  void editIap();
  void removeIap();
  void makeDefault();
  void setAutoConnect();
  void resetImsi(const QString& id);

private:
  Settings *m_settings;
  IapList *m_iaps;
  QCheckBox *m_auto;
};

#endif /* SETTINGS_DIALOG_HH */
