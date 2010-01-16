#ifndef IAP_DIALOG_HH
#define IAP_DIALOG_HH

#include <QDialog>
#include "iap.hh"

class QLineEdit;

class IapDialog : public QDialog {
  Q_OBJECT

public:
  IapDialog(Iap& iap, QWidget *parent = 0);
  ~IapDialog();

  Iap iap();

signals:
  void resetImsi(const QString& iap);

private slots:
  void resetImsi();
  void saveClicked();

private:
  QLineEdit *m_name;
  QLineEdit *m_apn;
  QLineEdit *m_username;
  QLineEdit *m_password;
  QLineEdit *m_proxyHost;
  QLineEdit *m_proxyPort;
  QLineEdit *m_mmscHost;
  QLineEdit *m_mmscPort;
};

#endif /* IAP_DIALOG_HH */
