#ifndef MMS_MESSAGE_HH
#define MMS_MESSAGE_HH

#include <QString>
#include <QStringList>
#include <QDateTime>

class QIODevice;
class MmsPart;
class QWspPduDecoder;
class QWspPduEncoder;

class MmsMessage {
public:
  enum MessageType {
    Invalid,
    SendReq,
    //    SendConf,
    //    Notification,
  };

  enum MessagePriority {
    PriorityUnset,
    PriorityLow,
    PriorityNormal,
    PriorityHigh,
  };

  MmsMessage(const MessageType& type);

  MmsMessage(QIODevice *in);
  bool encode(QIODevice *out);

  bool isValid() { return m_type != Invalid; }

  MessageType type() { return m_type; }
  QString transactionId() { return m_transactionId; }
  QString version() { return m_version; }

  QString from() { return m_from; }
  void setFrom(const QString& from) { m_from = from; }

  QStringList to() { return m_to; }
  void setTo(const QStringList& to) { m_to = to; }

  QStringList cc() { return m_cc; }
  void setCc(const QStringList& cc) { m_cc = cc; }

  QStringList bcc() { return m_bcc; }
  void setBcc(const QStringList& bcc) { m_bcc = bcc; }

  QString subject() { return m_subject; }
  void setSubject(const QString& subject) { m_subject = subject; }

  QString contentType() { return m_contentType; }
  void setContentType(const QString& contentType) { m_contentType = contentType; }

  bool isContentRelated() { return m_contentType.indexOf("multipart.related") != -1; }
  void setContentRelated(bool related);

  QList<MmsPart> parts();

  void addPart(const MmsPart& part);

  QDateTime date() { return m_date; }
  void setDate(QDateTime& date) { m_date = date; }

  bool isDeliveryReportEnabled() { return m_deliveryReport; }
  void setDeliveryReportEnabled(bool enabled) { m_deliveryReport = enabled; }

  bool isReadReplyEnabled() { return m_readReplyEnabled; }
  void setReadReplyEnabled(bool enabled) { m_readReplyEnabled = enabled; }

  MessagePriority messagePriority() { return m_priority; }
  void setMessagePriority(const MessagePriority& priority) { m_priority = priority; }

  QString messageClass() { return m_messageClass; }
  void setMessageClass(const QString& messageClass) { m_messageClass = messageClass; }

private:
  bool decodeHeader(QWspPduDecoder& d);
  bool encodeHeader(QWspPduEncoder& e);

  QList<MmsPart> m_parts;

  MessageType m_type;
  QString m_transactionId;
  QString m_version;
  QString m_from;
  QStringList m_to;
  QStringList m_cc;
  QStringList m_bcc;
  QString m_subject;
  QString m_contentType;
  QDateTime m_date;
  bool m_deliveryReport;
  bool m_readReplyEnabled;
  QString m_messageClass;
  MessagePriority m_priority;
};

#endif /* MMS_MESSAGE_HH */
