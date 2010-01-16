#include "incoming.hh"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include "folderview.hh"
#include <QMessageBox>
#include "folderviewitemformatter.hh"
#include "mmsmessage.hh"

class FromFormatter : public FolderViewItemFormatter {
public:
  QString format(const MmsMessage& message) {
    // TODO:
    return message.from();
  }
};

Incoming::Incoming(MmsWindow *parent) : MmsWindow(parent) {
  setWindowTitle(tr("Multimedia Messaging - Incoming"));
  m_formatter = new FromFormatter;
}

Incoming::~Incoming() {
  delete m_view;
  delete m_formatter;
}

void Incoming::doCreateContent() {
  m_download = menuBar()->addAction(tr("Download"), this, SLOT(downloadSelected()));
  m_delete = menuBar()->addAction(tr("Delete"), this, SLOT(deleteSelected()));
  m_remote = menuBar()->addAction(tr("Check New"), this, SLOT(checkNew()));

  m_view = new FolderView("Incoming", m_formatter, this);
  setCentralWidget(m_view);
}

void Incoming::deleteSelected() {
  QString id = m_view->selectedId();
  if (id.isEmpty()) {
    return;
  }

  if (QMessageBox::question(this, tr("Delete Message ?"), tr("This operation cannot be undone."),
			    QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
    m_view->deleteMessage(id);
  }
}
