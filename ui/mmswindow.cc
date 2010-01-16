#include "mmswindow.hh"
#include <QMenuBar>

MmsWindow::MmsWindow(MmsWindow *parent) : QMainWindow(parent), m_created(false) {

}

MmsWindow::~MmsWindow() {

}

void MmsWindow::createContent() {
  if (m_created) {
    return;
  }

  menuBar()->addAction(tr("Create New"), this, SIGNAL(createNew()));
  menuBar()->addAction(tr("Settings"), this, SIGNAL(showSettings()));

  doCreateContent();

  m_created = true;
}
