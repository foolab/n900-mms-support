#include <QApplication>
#include <QStringList>
#include <QDebug>
#include "smilviewer.hh"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QStringList args = app.arguments();
  if (args.size() != 2) {
    qWarning() << "Pass 1 file only";
    return 1;
  }

  SmilViewer viewer(args.at(1));

  viewer.show();

  return app.exec();
}
