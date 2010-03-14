#include "mmsmessage.hh"
#include "mmspart.hh"
#include <QDebug>
#include <QFile>
#include <QCoreApplication>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  if (app.arguments().size() != 2) {
    qDebug() << "Plese pass only 1 file";
    return 1;
  }

  QFile f(app.arguments().at(1));

  if (!f.open(QFile::ReadOnly)) {
    qDebug() << "Failed to open " << f.fileName() << f.errorString();
    return 1;
  }

  MmsMessage m(&f);

  qDebug() << "==== Message info: ====";
  qDebug() << "Valid:" << m.isValid();
  qDebug() << "Type:" << m.type();
  qDebug() << "Transaction ID:" << m.transactionId();
  qDebug() << "Version:" << m.version();
  qDebug() << "From:" << m.from();
  qDebug() << "To:" << m.to();
  qDebug() << "CC:" << m.cc();
  qDebug() << "BCC:" << m.bcc();
  qDebug() << "Subject:" << m.subject();
  qDebug() << "Content type:" << m.contentType();
  qDebug() << "Related content (Has SMIL):" << m.isContentRelated();
  qDebug() << "Date:" << m.date();
  qDebug() << "Delivery report enabled:" << m.isDeliveryReportEnabled();
  qDebug() << "Read reply enabled:" << m.isReadReplyEnabled();
  qDebug() << "Message class:" << m.messageClass();
  qDebug() << "Priority:" << m.messagePriority();
  qDebug() << "Size:" << m.size();
  qDebug() << "Location:" << m.location();
  qDebug() << "Expiry:" << m.expiry();
  qDebug() << "Message ID:" << m.messageId();

  QList<MmsPart> parts = m.parts();
  qDebug() << "Parts:" << parts.size();


  for (int x = 0; x < parts.size(); x++) {
    qDebug() << "  ==== Part" << x << "====";
    qDebug() << "  Content type:" << parts.at(x).contentType();
    qDebug() << "  Character set:" << parts.at(x).charset();
    qDebug() << "  Content ID" << parts.at(x).contentId();
    qDebug() << "  Is text:" << parts.at(x).isText();
    qDebug() << "  Is SMIL:" << parts.at(x).isSmil();
    qDebug() << "  Suggested file name:" << parts.at(x).suggestedName();

    if (parts.at(x).isText()) {
      qDebug() << "    ==== Text: ====";
      qDebug() << parts.at(x).data();
    }

    if (parts.at(x).isSmil()) {
      qDebug() << "    ==== SMIL: ====";
      qDebug() << parts.at(x).data();
    }
  }

  return 0;
}
