#include "mmsmessage.hh"
#include "mmspart.hh"
#include <QFile>
#include <QCoreApplication>
#include <QBuffer>
#include <QDebug>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  QStringList args = app.arguments();
  if (args.size() != 5) {
    qWarning() << "Usage" << args.at(0) << "<to> <file> <out> <subject>";
    return 0;
  }

  QString to = args.at(1);
  QString out = args.at(3);
  QString subject = args.at(4);

  QString type;
  if (args.at(2).endsWith(".jpg") || args.at(2).endsWith(".jpeg")) {
    type = "image/jpeg";
  }
  else if (args.at(2).endsWith(".gif")) {
    type = "image/gif";
  }
  else if (args.at(2).endsWith(".png")) {
    type = "image/png";
  }
  else if (args.at(2).endsWith(".txt") || args.at(2).endsWith(".text")) {
    type = "text/plain";
  }
  else {
    qWarning() << "Only JPEG, PNG, GIF and TXT are supported";
    return 1;
  }

  QFile file(args.at(2));
  if (!file.open(QFile::ReadOnly)) {
    qWarning() << "Failed to open" << args.at(2) << file.errorString();
    return 1;
  }

  MmsMessage m = MmsMessage(MmsMessage::SendReq);
  m.setTo(QStringList() << to);
  m.setSubject(subject);
  m.setContentRelated(false);

  MmsPart p(type, file.readAll());
  m.addPart(p);

  QFile o(out);
  if (!o.open(QFile::WriteOnly | QFile::Truncate)) {
    qWarning() << "Failed to open" << out << o.errorString();
    return 1;
  }

  m.encode(&o);

  o.flush();
  o.close();

  if (o.error() != QFile::NoError) {
    qWarning() << "Failed to write" << out << o.errorString();
    return 1;
  }

  return 0;
}
