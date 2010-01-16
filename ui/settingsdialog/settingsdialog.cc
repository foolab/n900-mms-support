#include "settingsdialog.hh"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVariant>
#include "iaplist.hh"
#include <QGridLayout>
#include "settings.hh"
#include <QMessageBox>
#include "iapdialog.hh"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle(tr("Configure Multimedia Messaging"));
  m_settings = new Settings(this);

  QGridLayout *layout = new QGridLayout(this);

  m_iaps = new IapList(m_settings, this);
  m_iaps->setProperty("FingerScrollable", true);
  layout->addWidget(m_iaps, 0, 0, 1, 1);

  m_auto = new QCheckBox(tr("Connect when needed."), this);
  m_auto->setChecked(m_settings->autoConnect());
  layout->addWidget(m_auto, 1, 0, 1, 1);

  QDialogButtonBox *box = new QDialogButtonBox(Qt::Vertical, this);
  QObject::connect(box, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(box, SIGNAL(rejected()), this, SLOT(reject()));

  QPushButton *add = new QPushButton(tr("Add"), this);
  QPushButton *edit = new QPushButton(tr("Edit"), this);
  QPushButton *remove = new QPushButton(tr("Remove"), this);
  QPushButton *makeDefault = new QPushButton(tr("Default"), this);

  box->addButton(add, QDialogButtonBox::ActionRole);
  box->addButton(edit, QDialogButtonBox::ActionRole);
  box->addButton(remove, QDialogButtonBox::ActionRole);
  box->addButton(makeDefault, QDialogButtonBox::ActionRole);

  layout->addWidget(box, 0, 1, 1, 2);

  QObject::connect(add, SIGNAL(clicked()), this, SLOT(addIap()));
  QObject::connect(edit, SIGNAL(clicked()), this, SLOT(editIap()));
  QObject::connect(remove, SIGNAL(clicked()), this, SLOT(removeIap()));
  QObject::connect(makeDefault, SIGNAL(clicked()), this, SLOT(makeDefault()));
  QObject::connect(m_auto, SIGNAL(clicked()), this, SLOT(setAutoConnect()));
}

SettingsDialog::~SettingsDialog() {
  delete m_settings;
  m_settings = 0;
}

void SettingsDialog::addIap() {
  Iap iap;
  IapDialog d(iap, this);
  while (d.exec() == QDialog::Accepted) {
    if (!m_settings->addIap(d.iap())) {
      QMessageBox::critical(this, tr("Error Saving IAP"), tr("Failed to save IAP."), QMessageBox::Ok);
    }
    else {
      return;
    }
  }
}

void SettingsDialog::editIap() {
  QString iap = m_iaps->selectedIap();
  if (iap.isEmpty()) {
    return;
  }

  Iap i = m_settings->iap(iap);
  IapDialog d(i, this);
  QObject::connect(&d, SIGNAL(resetImsi(const QString&)), this, SLOT(resetImsi(const QString&)));

  while (d.exec() == QDialog::Accepted) {
    if (!m_settings->saveIap(d.iap())) {
      QMessageBox::critical(this, tr("Error Saving IAP"), tr("Failed to save IAP."), QMessageBox::Ok);
    }
    else {
      return;
    }
  }
}

void SettingsDialog::removeIap() {
  QString iap = m_iaps->selectedIap();
  if (iap.isEmpty()) {
    return;
  }

  if (QMessageBox::question(this,
			    tr("Delete Access Point"), tr("Delete access point %1 ?").arg(iap),
			    QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {

    m_settings->removeIap(iap);
  }
}

void SettingsDialog::makeDefault() {
  QString iap = m_iaps->selectedIap();
  if (iap.isEmpty()) {
    return;
  }

  m_settings->setDefaultIap(iap);
}

void SettingsDialog::setAutoConnect() {
  m_settings->setAutoConnect(m_auto->isChecked());
}

void SettingsDialog::resetImsi(const QString& id) {
  if (!m_settings->resetImsi(id)) {
    QMessageBox::critical(this,
			  tr("Failed to reset IMSI"), tr("Failed to reset IMSI for %1").arg(id),
			  QMessageBox::Ok);
  }
}
