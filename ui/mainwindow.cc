#include "mainwindow.hh"
#include <QListWidget>

MainWindow::MainWindow() {
  setWindowTitle(tr("Multimedia Messaging"));
}

MainWindow::~MainWindow() {

}

void MainWindow::doCreateContent() {
  QListWidget *widget = new QListWidget(this);
  QObject::connect(widget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemClicked(QListWidgetItem *)));
  widget->setProperty("FingerScrollable", true);
  setCentralWidget(widget);

  QListWidgetItem *item = 0;

  item = new QListWidgetItem(tr("Inbox"), widget);
  item->setData(Qt::UserRole, InboxWindow);
  item->setTextAlignment(Qt::AlignCenter);

  item = new QListWidgetItem(tr("Outbox"), widget);
  item->setData(Qt::UserRole, OutboxWindow);
  item->setTextAlignment(Qt::AlignCenter);

  item = new QListWidgetItem(tr("Sent"), widget);
  item->setData(Qt::UserRole, SentWindow);
  item->setTextAlignment(Qt::AlignCenter);

  item = new QListWidgetItem(tr("Reports"), widget);
  item->setData(Qt::UserRole, ReportsWindow);
  item->setTextAlignment(Qt::AlignCenter);

  item = new QListWidgetItem(tr("Incoming"), widget);
  item->setData(Qt::UserRole, IncomingWindow);
  item->setTextAlignment(Qt::AlignCenter);

  item = new QListWidgetItem(tr("Drafts"), widget);
  item->setData(Qt::UserRole, DraftsWindow);
  item->setTextAlignment(Qt::AlignCenter);
}

void MainWindow::itemClicked(QListWidgetItem *item) {
  switch (item->data(Qt::UserRole).toInt()) {
  case OutboxWindow:
    emit showOutbox();
    break;
  case SentWindow:
    emit showSent();
    break;
  case ReportsWindow:
    emit showReports();
    break;
  case IncomingWindow:
    emit showIncoming();
    break;
  case DraftsWindow:
    emit showDrafts();
    break;
  case InboxWindow:
  default:
    emit showInbox();
    break;
  }
}
