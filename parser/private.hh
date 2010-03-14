#ifndef PRIVATE_HH
#define PRIVATE_HH

void setFrom(QWspPduEncoder& e, const QString& addr) {
  if (addr.isEmpty()) {
    e.encodeLength(1);
    e.encodeOctet(129); // insert address
  }
  else {
    quint32 len = addr.length()+2;
    e.encodeLength(len);
    e.encodeOctet(128);
    e.encodeTextString(addr);
  }
}

QString getFrom(QWspPduDecoder& d) {
  QString addr;
  quint32 len = d.decodeLength();
  int epos = d.device()->pos() + len;
  quint8 tok = d.decodeOctet();
  if (tok == 128) {
    addr = d.decodeEncodedString();
  } else if (tok == 129) {
    // nothing
  } else {
    qWarning("Invalid token in address field");
    d.setStatus(QWspPduDecoder::Unhandled);
  }
  while (d.device()->pos() < epos && !d.device()->atEnd()) {
    d.decodeOctet();
  }

  return addr;
}

void setMessageType(QWspPduEncoder& e, MmsMessage::MessageType& t) {
  switch (t) {
  case MmsMessage::SendReq:
    e.encodeUInt8(PDU_M_SEND_REQ);
    break;
  case MmsMessage::NotificationInd:
    e.encodeUInt8(PDU_M_NOTIFICATION_IND);
    break;
  case MmsMessage::SendConf:
    e.encodeUInt8(PDU_M_SEND_CONF);
    break;
  case MmsMessage::NotifyResp:
    e.encodeUInt8(PDU_M_NOTIFYRESP_IND);
    break;
  case MmsMessage::DeliveryReport:
    e.encodeUInt8(PDU_M_DELIVERY_IND);
  default: // TODO:
    return;
  }
}

MmsMessage::MessageType getMessageType(QWspPduDecoder& d) {
  quint8 t = d.decodeUInt8();
  //  qDebug() << "PDU" << QString("0x%1").arg(QString::number(t, 16));
  //  qDebug() << t;
  switch (t) {
  case PDU_M_SEND_REQ:
    return MmsMessage::SendReq;
  case PDU_M_NOTIFICATION_IND:
    return MmsMessage::NotificationInd;
  case PDU_M_SEND_CONF:
    return MmsMessage::SendConf;
  case PDU_M_NOTIFYRESP_IND:
    return MmsMessage::NotifyResp;
  case PDU_M_DELIVERY_IND:
    return MmsMessage::DeliveryReport;
  case PDU_M_RETRIEVE_CONF:
  case PDU_M_ACKNOWLEDGE_IND:

  case PDU_M_READ_REC_IND:
  case PDU_M_READ_ORIG_IND:
  case PDU_M_FORWARD_REQ:
  case PDU_M_FORWARD_CONF:
  case PDU_M_MBOX_STORE_REQ:
  case PDU_M_MBOX_STORE_CONF:
  case PDU_M_MBOX_VIEW_REQ:
  case PDU_M_MBOX_VIEW_CONF:
  case PDU_M_MBOX_UPLOAD_REQ:
  case PDU_M_MBOX_UPLOAD_CONF:
  case PDU_M_MBOX_DELETE_REQ:
  case PDU_M_MBOX_DELETE_CONF:
  case PDU_M_MBOX_DESCR:
  default:
    return MmsMessage::Invalid;
  }

  return MmsMessage::Invalid;
}

void setAddr(QWspPduEncoder& e, const QString& to) {
  QString addr = to;
  addr.append("/TYPE=PLMN");
  e.encodeEncodedString(addr);
}

QString getAddr(QWspPduDecoder& d) {
  QString addr = d.decodeEncodedString();

  int index = addr.indexOf('/');
  if (index != -1) {
    addr = addr.left(index);
  }

  return addr;
}

bool getBool(QWspPduDecoder& d) {
  quint8 o = d.decodeOctet();

  switch (o) {
  case 0x80: // octet 128
    return true;
  case 0x81: // octet 129
    return false;
  }

  qWarning() << "Unknown delivery report octet!" << o;

  return true;
}

void setBool(QWspPduEncoder& e, bool enabled) {
  if (enabled) {
    e.encodeOctet(0x80);
  }
  else {
    e.encodeOctet(0x81);
  }
}

QString getMessageClass(QWspPduDecoder& d) {
  quint8 o = d.peekOctet();
  switch (o) {
  case 0x80: // 128 Personal
    d.decodeOctet(); // eat it
    return "Personal";

  case 0x81: // 129 Advertisement
    d.decodeOctet(); // eat it
    return "Advertisement";

  case 0x82: // 130 Informational
    d.decodeOctet(); // eat it
    return "Informational";

  case 0x83: // 131 Auto
    d.decodeOctet(); // eat it
    return "Auto";

  default:
    break;
  }

  QString ret = d.decodeTextString();
  if (ret.isEmpty()) {
    return "Personal";
  }

  return ret;
}

void setMessageClass(QWspPduEncoder& e, const QString& mClass) {
  if (mClass == "Personal") {
    e.encodeOctet(0x80);
  }
  else if (mClass == "Advertisement") {
    e.encodeOctet(0x81);
  }
  else if (mClass == "Informational") {
    e.encodeOctet(0x82);
  }
  else if (mClass == "Auto") {
    e.encodeOctet(0x83);
  }
  else {
    e.encodeTextString(mClass.isEmpty() ? "Personal" : mClass);
  }
}

MmsMessage::MessagePriority getMessagePriority(QWspPduDecoder& d) {
  switch (d.decodeOctet()) {
  case 0x80: // 128 Low
    return MmsMessage::PriorityLow;
  case 0x81: // 129 Normal
    return MmsMessage::PriorityNormal;
  case 0x82: // 130 High
    return MmsMessage::PriorityHigh;
  }

  return MmsMessage::PriorityNormal;
}

void setMessagePriority(QWspPduEncoder& e, const MmsMessage::MessagePriority& priority) {
  switch (priority) {
  case MmsMessage::PriorityLow:
    e.encodeOctet(0x80);
    return;
  case MmsMessage::PriorityHigh:
    e.encodeOctet(0x82);
    return;
  case MmsMessage::PriorityNormal:
  default:
    e.encodeOctet(0x81);
    return;
  }
}

QDateTime getExpiry(QWspPduDecoder& d) {
  // Stolen from Qt-extended
  quint32 len = d.decodeLength();
  quint8 octet = d.decodeOctet();
  if (octet == 128) {
    // absolute
    quint32 i = d.decodeLongInteger();
    return QDateTime::fromTime_t(i);
  } else if (octet == 129) {
    // relative
    long s = d.decodeInteger();
    return QDateTime::currentDateTime().addSecs(s);
  }

  qWarning("Unknown token in expiry field");
  while (--len > 0) {
    d.decodeOctet();
  }

  return QDateTime();
}

MmsMessage::MessageStatus getMessageStatus(QWspPduDecoder& d) {
  switch (d.decodeOctet()) {
  case 128:
    return MmsMessage::StatusExpired;
  case 129:
    return MmsMessage::StatusRetrieved;
  case 130:
    return MmsMessage::StatusRejected;
  case 131:
    return MmsMessage::StatusDeferred;
  case 132:
    return MmsMessage::StatusUnrecognised;
  case 133:
    return MmsMessage::StatusIndeterminate;
  case 134:
    return MmsMessage::StatusForwarded;
  case 135:
    return MmsMessage::StatusUnreachable;
  default:
    return MmsMessage::StatusUnrecognised;
  }
}

void setMessageStatus(QWspPduEncoder& e, const MmsMessage::MessageStatus& status) {
  switch (status) {
  case MmsMessage::StatusExpired:
    e.encodeOctet(128);
    break;
  case MmsMessage::StatusRetrieved:
    e.encodeOctet(129);
    break;
  case MmsMessage::StatusRejected:
    e.encodeOctet(130);
    break;
  case MmsMessage::StatusDeferred:
    e.encodeOctet(131);
    break;
  case MmsMessage::StatusUnrecognised:
    e.encodeOctet(132);
    break;
  case MmsMessage::StatusIndeterminate:
    e.encodeOctet(133);
    break;
  case MmsMessage::StatusForwarded:
    e.encodeOctet(134);
    break;
  case MmsMessage::StatusUnreachable:
    e.encodeOctet(135);
    break;
  }
}

#endif /* PRIVATE_HH */
