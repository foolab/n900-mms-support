#include "iapdialog.hh"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QVariant>
#include <QPushButton>
#include <QLineEdit>
#include <QIntValidator>
#include <QMessageBox>

IapDialog::IapDialog(Iap& iap, QWidget *parent) : QDialog(parent) {
  setWindowTitle(iap.name().isEmpty() ? tr("Add New IAP") : tr("Edit IAP %1").arg(iap.name()));

  QWidget *widget = new QWidget(this);
  QFormLayout *layout = new QFormLayout(widget);

  m_name = new QLineEdit(iap.name(), this);
  m_name->setEnabled(iap.name().isEmpty());
  layout->addRow(tr("Connection Name:"), m_name);

  m_apn = new QLineEdit(iap.apn(), this);
  layout->addRow(tr("APN Name:"), m_apn);

  m_username = new QLineEdit(iap.username(), this);
  layout->addRow(tr("Username:"), m_username);

  m_password = new QLineEdit(iap.password(), this);
  m_password->setEchoMode(QLineEdit::Password);
  layout->addRow(tr("Password:"), m_password);

  m_mmscHost = new QLineEdit(iap.mmscHost(), this);
  layout->addRow(tr("MMSC Host:"), m_mmscHost);

  m_mmscPort = new QLineEdit(this);
  layout->addRow(tr("MMSC Port:"), m_mmscPort);
  QIntValidator *mp = new QIntValidator(m_mmscPort);
  mp->setRange(0, 65535);
  m_mmscPort->setValidator(mp);

  m_proxyHost = new QLineEdit(iap.proxyHost(), this);
  layout->addRow(tr("Proxy Host:"), m_proxyHost);

  m_proxyPort = new QLineEdit(this);
  layout->addRow(tr("Proxy Port:"), m_proxyPort);
  QIntValidator *pp = new QIntValidator(m_proxyPort);
  pp->setRange(0, 65535);
  m_proxyPort->setValidator(pp);

  QHBoxLayout *mainLayout = new QHBoxLayout(this);
  QScrollArea *area = new QScrollArea(this);
  area->setProperty("FingerScrollable", true);

  mainLayout->addWidget(area);
  area->setWidget(widget);

  QDialogButtonBox *box = new QDialogButtonBox(Qt::Vertical, this);
  QObject::connect(box, SIGNAL(accepted()), this, SLOT(saveClicked()));
  QObject::connect(box, SIGNAL(rejected()), this, SLOT(reject()));

  QPushButton *save = new QPushButton(tr("Save"), this);
  save->setDefault(true);

  QPushButton *cancel = new QPushButton(tr("Cancel"), this);

  if (!iap.name().isEmpty()) {
    QPushButton *imsi = new QPushButton(tr("Reset IMSI"), this);
    QObject::connect(imsi, SIGNAL(clicked()), this, SLOT(resetImsi()));
    box->addButton(imsi, QDialogButtonBox::ActionRole);
  }

  box->addButton(save, QDialogButtonBox::AcceptRole);
  box->addButton(cancel, QDialogButtonBox::RejectRole);

  mainLayout->addWidget(box);
}

IapDialog::~IapDialog() {

}

Iap IapDialog::iap() {
  Iap iap;

  iap.setName(m_name->text());
  iap.setApn(m_apn->text());
  iap.setUsername(m_username->text());
  iap.setPassword(m_password->text());
  iap.setProxyHost(m_proxyHost->text());
  iap.setProxyPort(m_proxyPort->text().toInt());
  iap.setMmscHost(m_mmscHost->text());
  iap.setMmscPort(m_mmscPort->text().toInt());

  return iap;
}

void IapDialog::resetImsi() {
  QString name = m_name->text();
  if (!name.isEmpty()) {
    emit resetImsi(name);
  }
}

void IapDialog::saveClicked() {
  Iap i = iap();
  if (i.name().isEmpty()) {
    QMessageBox::critical(this, tr("Error Saving IAP"), tr("IAP name is required."), QMessageBox::Ok);
    return;
  }
  else if (i.apn().isEmpty()) {
    QMessageBox::critical(this, tr("Error Saving IAP"), tr("APN is required."), QMessageBox::Ok);
    return;
  }
  else if (i.mmscHost().isEmpty()) {
    QMessageBox::critical(this, tr("Error Saving IAP"), tr("MMSC host is required."), QMessageBox::Ok);
    return;
  }

  accept();
}
