#ifndef MMS_MESSAGE_HH
#define MMS_MESSAGE_HH

#include <QString>
#include <QStringList>
#include <QDateTime>
#include "mmspart.hh"

class QIODevice;
class QWspPduDecoder;
class QWspPduEncoder;

class MmsMessage {

public:
  enum MessageType {
    Invalid,
    SendReq,
    NotificationInd,
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

  bool isValid() const { return m_type != Invalid; }

  MessageType type() const { return m_type; }

  QString transactionId() const { return m_transactionId; }
  QString version() { return m_version; }

  QString from() const { return m_from; }
  void setFrom(const QString& from) { m_from = from; }

  QStringList to() const { return m_to; }
  void setTo(const QStringList& to) { m_to = to; }

  QStringList cc() const { return m_cc; }
  void setCc(const QStringList& cc) { m_cc = cc; }

  QStringList bcc() const { return m_bcc; }
  void setBcc(const QStringList& bcc) { m_bcc = bcc; }

  QString subject() const { return m_subject; }
  void setSubject(const QString& subject) { m_subject = subject; }

  QString contentType() const { return m_contentType; }
  void setContentType(const QString& contentType) { m_contentType = contentType; }

  bool isContentRelated() const { return m_contentType.indexOf("multipart.related") != -1; }
  void setContentRelated(bool related);

  QList<MmsPart> parts();

  void addPart(const MmsPart& part);

  QDateTime date() const { return m_date; }
  void setDate(QDateTime& date) { m_date = date; }

  bool isDeliveryReportEnabled() const { return m_deliveryReport; }
  void setDeliveryReportEnabled(bool enabled) { m_deliveryReport = enabled; }

  bool isReadReplyEnabled() const { return m_readReplyEnabled; }
  void setReadReplyEnabled(bool enabled) { m_readReplyEnabled = enabled; }

  MessagePriority messagePriority() const { return m_priority; }
  void setMessagePriority(const MessagePriority& priority) { m_priority = priority; }

  QString messageClass() const { return m_messageClass; }
  void setMessageClass(const QString& messageClass) { m_messageClass = messageClass; }

  long size() const { return m_size; }

  QString location() const { return m_location; }

  QDateTime expiry() const { return m_expiry; }

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
  long m_size;
  QString m_location;
  QDateTime m_expiry;
};

#endif /* MMS_MESSAGE_HH */
