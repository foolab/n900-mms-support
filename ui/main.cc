#include <QApplication>
#include "mmsui.hh"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MmsUi ui;

  ui.init();

  return app.exec();
}
