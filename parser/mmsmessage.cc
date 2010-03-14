#include "mmsmessage.hh"
#include <QDataStream>
#include <QDebug>
#include "types.h"
#include "qwsppdu.h"
#include "private.hh"
#include "mmspart.hh"
#include <QFile>

MmsMessage::MmsMessage(const MessageType& type) {
  m_version = "1.0";
  m_deliveryReport = true;
  m_readReplyEnabled = true;
  m_messageClass = "Personal";
  m_priority = PriorityUnset;
  m_type = type;
  m_size = 0;
  m_status = StatusRetrieved;
}

MmsMessage::MmsMessage(QIODevice *in) {
  m_version = "1.0";
  m_deliveryReport = true;
  m_readReplyEnabled = true;
  m_messageClass = "Personal";
  m_priority = PriorityUnset;
  m_size = 0;

  m_status = StatusRetrieved;

  QWspPduDecoder d(in);

  if (!decodeHeader(d)) {
    m_type = Invalid;
    return;
  }

  //  quint8 pdu = d.decodeOctet();
  //  qDebug() << pdu;

  QWspMultipart p = d.decodeMultipart();
  for (int x = 0; x < p.count(); x++) {
    m_parts.append(MmsPart(p.part(x)));
  }
}

MmsMessage::MmsMessage(const QString& path) {
  m_version = "1.0";
  m_deliveryReport = true;
  m_readReplyEnabled = true;
  m_messageClass = "Personal";
  m_priority = PriorityUnset;
  m_size = 0;
  m_type = Invalid;
  m_status = StatusRetrieved;

  QFile f(path);
  if (!f.open(QFile::ReadOnly)) {
    qWarning() << "MmsMessage: Failed to open" << f.fileName() << f.errorString();
    return;
  }

  QWspPduDecoder d(&f);

  if (!decodeHeader(d)) {
    m_type = Invalid;
    return;
  }

  //  quint8 pdu = d.decodeOctet();
  //  qDebug() << pdu;

  QWspMultipart p = d.decodeMultipart();
  for (int x = 0; x < p.count(); x++) {
    m_parts.append(MmsPart(p.part(x)));
  }
}

bool MmsMessage::decodeHeader(QWspPduDecoder& d) {
  while (!d.device()->atEnd()) {
    quint8 pdu = d.decodeOctet();

    //    qDebug() << "PDU:" << QString("0x%1").arg(QString::number((unsigned int)pdu, 16));

    switch (pdu) {
    case MM_MTYPE_HDR:
      qDebug() << "MM_MTYPE_HDR";
      m_type = getMessageType(d);
      break;
    case MM_TID_HDR:
      qDebug() << "MM_TID_HDR";
      m_transactionId = d.decodeTextString();
      break;
    case MM_VERSION_HDR:
      qDebug() << "MM_VERSION_HDR";
      m_version = d.decodeVersion();
      break;
    case MM_FROM_HDR:
      //      qDebug() << "MM_FROM_HDR";
      m_from = getFrom(d);
      break;
    case MM_TO_HDR:
      //      qDebug() << "MM_TO_HDR";
      m_to.append(getAddr(d));
      break;
    case MM_CTYPE_HDR:
      //      qDebug() << "MM_CTYPE_HDR";
      m_contentType = d.decodeContentType();
      // This should be the last of the headers.
      return true;
    case MM_SUBJECT_HDR:
      //      qDebug() << "MM_SUBJECT_HDR";
      m_subject = d.decodeEncodedString();
      break;
    case MM_DATE_HDR:
      //      qDebug() << "MM_DATE_HDR";
      m_date = QDateTime::fromTime_t(d.decodeLongInteger());
      break;
    case MM_DREPORT_HDR:
      //      qDebug() << "MM_DREPORT_HDR";
      m_deliveryReport = getBool(d);
      break;
    case MM_RREPLY_HDR:
      //      qDebug() << "MM_RREPLY_HDR";
      m_readReplyEnabled = getBool(d);
      break;
    case MM_MCLASS_HDR:
      //      qDebug() << "MM_MCLASS_HDR";
      m_messageClass = getMessageClass(d);
      break;
    case MM_PRIORITY_HDR:
      //      qDebug() << "MM_PRIORITY_HDR";
      m_priority = getMessagePriority(d);
      break;
    case MM_MSIZE_HDR:
      //      qDebug() << "MM_MSIZE_HDR";
      m_size = d.decodeLongInteger();
      break;
    case MM_EXPIRY_HDR:
      //      qDebug() << "MM_EXPIRY_HDR";
      m_expiry = getExpiry(d);
      break;
    case MM_CLOCATION_HDR:
      //      qDebug() << "MM_CLOCATION_HDR";
      m_location = d.decodeTextString();
      break;
    case MM_STATUS_HDR:
      qDebug() << "MM_STATUS_HDR";
      m_status = getMessageStatus(d);
      break;
    case MM_MID_HDR:
      qDebug() << "MM_MID_HDR";
      m_messageId = d.decodeTextString();
      break;
    case MM_RTEXT_HDR: // TODO:
      qDebug() << "MM_RTEXT_HDR";
    case MM_RSTATUS_HDR: // TODO:
      qDebug() << "MM_RSTATUS_HDR";
    case MM_STORE_STATUS_HDR: // TODO:
      qDebug() << "MM_STORE_STATUS_HDR";
    case MM_STORE_STATUS_TEXT_HDR: // TODO:
      qDebug() << "MM_STORE_STATUS_TEXT_HDR";
    default:
      qDebug() << "Unknown PDU" << QString("0x%1").arg(QString::number((unsigned int)pdu, 16));
      //      break;
      return false;
    }
  }

  // TODO:
  return true;
}

QList<MmsPart> MmsMessage::parts() {
  return m_parts;
}

void MmsMessage::addPart(const MmsPart& part) {
  m_parts.append(part);
}

void MmsMessage::setContentRelated(bool related) {
  if (related) {
    m_contentType = "application/vnd.wap.multipart.related";
  }
  else {
    m_contentType = "application/vnd.wap.multipart.mixed";
  }
}

bool MmsMessage::encode(QIODevice *out) {
  if (!isValid()) {
    qWarning() << "MmsMessage::encode invalid message";
    return false;
  }

  if (m_type == MmsMessage::SendReq && m_contentType.isEmpty()) {
    qWarning() << "MmsMessage::encode no content type.";
    return false;
  }

  /*
  if (m_parts.isEmpty()) {
    qWarning() << "MmsMessage::encode No parts added.";
    return false;
  }
  */

  if (isContentRelated()) {
    bool hasSmil = false;

    for (int x = 0; x < m_parts.size(); x++) {
      bool smil = m_parts.at(x).isSmil();
      if (smil) {
	hasSmil = smil;
	break;
      }
    }

    if (!hasSmil) {
      qWarning() << "MmsMessage::encode related content without SMIL";
      return false;
    }
  }

  if (m_transactionId.isEmpty()) {
    //    m_transactionId = "ABC";
    m_transactionId = QString("%1").arg(QDateTime::currentDateTime().toTime_t());
  }

  QWspPduEncoder e(out);

  if (!encodeHeader(e)) {
    return false;
  }

  //  int smil = -1
  // TODO: encode smil 1st ?

  if (m_type == MmsMessage::SendReq) {
    e.encodeUIntVar(m_parts.size());
    for (int x = 0; x < m_parts.size(); x++) {
      m_parts.at(x).encode(e);
    }
  }

  return true;
}

bool MmsMessage::encodeHeader(QWspPduEncoder& e) {
  switch (m_type) {
  case MmsMessage::SendReq:
    return encodeSendReq(e);
  case MmsMessage::NotifyResp:
    return encodeNotifyResp(e);
  default:
    qWarning() << "MmsMessage::encodeHeader" << m_type << "unsupported!";
    return false;
  }
}

bool MmsMessage::encodeSendReq(QWspPduEncoder& e) {
  // X-Mms-Message-Type
  e.encodeOctet(MM_MTYPE_HDR);
  setMessageType(e, m_type);

  // X-Mms-Transaction-ID
  e.encodeOctet(MM_TID_HDR);
  e.encodeTextString(m_transactionId);

  // X-Mms-MMS-Version
  e.encodeOctet(MM_VERSION_HDR);
  e.encodeVersion(m_version);

  // Now the rest:
  if (m_date.isValid()) {
    e.encodeOctet(MM_DATE_HDR);
    e.encodeLongInteger(m_date.toTime_t());
  }

  e.encodeOctet(MM_DREPORT_HDR);
  setBool(e, m_deliveryReport);

  e.encodeOctet(MM_RREPLY_HDR);
  setBool(e, m_readReplyEnabled);

  if (!m_messageClass.isEmpty()) {
    e.encodeOctet(MM_MCLASS_HDR);
    ::setMessageClass(e, m_messageClass);
  }

  if (m_priority == PriorityLow || m_priority == PriorityNormal || m_priority == PriorityHigh) {
    e.encodeOctet(MM_PRIORITY_HDR);
    ::setMessagePriority(e, m_priority);
  }

  e.encodeOctet(MM_FROM_HDR);
  ::setFrom(e, m_from);

  for (int x = 0; x < m_to.size(); x++) {
    e.encodeOctet(MM_TO_HDR);
    setAddr(e, m_to.at(x));
  }

  if (!m_subject.isEmpty()) {
    e.encodeOctet(MM_SUBJECT_HDR);
    e.encodeEncodedString(m_subject);
  }

  for (int x = 0; x < m_cc.size(); x++) {
    e.encodeOctet(MM_CC_HDR);
    setAddr(e, m_cc.at(x));
  }

  for (int x = 0; x < m_bcc.size(); x++) {
    e.encodeOctet(MM_BCC_HDR);
    setAddr(e, m_bcc.at(x));
  }

  // Last:
  e.encodeOctet(MM_CTYPE_HDR);
  e.encodeContentType(m_contentType);

  return true;
}

bool MmsMessage::encodeNotifyResp(QWspPduEncoder& e) {
  if (m_transactionId.isEmpty()) {
    qWarning() << "MmsMessage::encodeNotifyResp empty transaction ID";
    return false;
  }

  // X-Mms-Message-Type
  e.encodeOctet(MM_MTYPE_HDR);
  setMessageType(e, m_type);

  // X-Mms-Transaction-ID
  e.encodeOctet(MM_TID_HDR);
  e.encodeTextString(m_transactionId);

  // X-Mms-MMS-Version
  e.encodeOctet(MM_VERSION_HDR);
  e.encodeVersion(m_version);

  // X-Mms-Status
  e.encodeOctet(MM_STATUS_HDR);
  ::setMessageStatus(e, m_status);

  return true;
}
