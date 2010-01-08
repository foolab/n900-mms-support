#include "mmsmessage.hh"
#include <QDataStream>
#include <QDebug>
#include "types.h"
#include "qwsppdu.h"
#include "private.hh"
#include "mmspart.hh"
#include <QCryptographicHash>

MmsMessage::MmsMessage(const MessageType& type) {
  m_version = "1.0";
  m_deliveryReport = true;
  m_readReplyEnabled = true;
  m_messageClass = "Personal";
  m_priority = PriorityUnset;
  m_type = type;
}

MmsMessage::MmsMessage(QIODevice *in) {
  m_version = "1.0";
  m_deliveryReport = true;
  m_readReplyEnabled = true;
  m_messageClass = "Personal";
  m_priority = PriorityUnset;

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

bool MmsMessage::decodeHeader(QWspPduDecoder& d) {
  while (!d.device()->atEnd()) {
    quint8 pdu = d.decodeOctet();

    switch (pdu) {
    case MM_MTYPE_HDR:
      //      qDebug() << "MM_MTYPE_HDR";
      m_type = getMessageType(d);
      break;
    case MM_TID_HDR:
      //      qDebug() << "MM_TID_HDR";
      m_transactionId = d.decodeTextString();
      break;
    case MM_VERSION_HDR:
      //      qDebug() << "MM_VERSION_HDR";
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

    default:
      qDebug() << "Unknown PDU" << QString("0x%1").arg(QString::number((unsigned int)pdu, 16));
      return false;
    }
  }

  return false;
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

  if (m_contentType.isEmpty()) {
    qWarning() << "MmsMessage::encode no content type.";
    return false;
  }

  if (m_parts.isEmpty()) {
    qWarning() << "MmsMessage::encode No parts added.";
    return false;
  }

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
    QByteArray data;
    for (int x = 0; x < m_parts.size(); x++) {
      data.append(m_parts.at(x).data());
    }

    if (data.isEmpty()) {
      qWarning() << "MmsMessage::encode No data.";
      return false;
    }

    QCryptographicHash h(QCryptographicHash::Md5);
    h.addData(data);
    QByteArray r = h.result();

    m_transactionId = r.toHex();
  }

  QWspPduEncoder e(out);

  if (!encodeHeader(e)) {
    return false;
  }

  //  int smil = -1
  // TODO: encode smil 1st ?

  e.encodeUIntVar(m_parts.size());
  for (int x = 0; x < m_parts.size(); x++) {
    m_parts.at(x).encode(e);
  }

  return true;
}

bool MmsMessage::encodeHeader(QWspPduEncoder& e) {
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
