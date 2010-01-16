#include "mmsui.hh"
#include <QMainWindow>
#include "mainwindow.hh"
#include "settingsdialog.hh"
#include "incoming.hh"

MmsUi::MmsUi(QObject *parent) :
  QObject(parent) {

}

MmsUi::~MmsUi() {

}

void MmsUi::init() {
  m_main = new MainWindow;
  m_main->createContent();
  m_main->show();

  QObject::connect(m_main, SIGNAL(createNew()), this, SLOT(createNew()));
  QObject::connect(m_main, SIGNAL(showSettings()), this, SLOT(showSettings()));
  QObject::connect(m_main, SIGNAL(showInbox()), this, SLOT(showInbox()));
  QObject::connect(m_main, SIGNAL(showOutbox()), this, SLOT(showOutbox()));
  QObject::connect(m_main, SIGNAL(showReports()), this, SLOT(showReports()));
  QObject::connect(m_main, SIGNAL(showSent()), this, SLOT(showSent()));
  QObject::connect(m_main, SIGNAL(showIncoming()), this, SLOT(showIncoming()));
  QObject::connect(m_main, SIGNAL(showDrafts()), this, SLOT(showDrafts()));
}

MmsWindow *MmsUi::setup(MmsWindow *window) {
  window->setAttribute(Qt::WA_DeleteOnClose);
  window->createContent();

  QObject::connect(window, SIGNAL(showSettings()), this, SLOT(showSettings()));

  return window;
}

void MmsUi::showSettings() {
  SettingsDialog d;
  d.exec();
}

void MmsUi::createNew() {
  // TODO:
}

void MmsUi::showInbox() {
  // TODO:
}

void MmsUi::showOutbox() {
  // TODO:
}

void MmsUi::showReports() {
  // TODO:
}

void MmsUi::showSent() {
  // TODO:
}

void MmsUi::showIncoming() {
  if (!m_incoming) {
    m_incoming = setup(new Incoming(m_main));
  }

  m_incoming->show();
}

void MmsUi::showDrafts() {
  // TODO:
}
