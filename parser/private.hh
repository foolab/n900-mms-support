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
  default: // TODO:
    return;
  }
}

MmsMessage::MessageType getMessageType(QWspPduDecoder& d) {
  quint8 t = d.decodeUInt8();
  //qDebug() << "PDU" << QString("0x%1").arg(QString::number(t, 16));
  //  qDebug() << t;
  switch (t) {
  case PDU_M_SEND_REQ:
    return MmsMessage::SendReq;
  case PDU_M_SEND_CONF:
  case PDU_M_NOTIFICATION_IND:
  case PDU_M_NOTIFYRESP_IND:
  case PDU_M_RETRIEVE_CONF:
  case PDU_M_ACKNOWLEDGE_IND:
  case PDU_M_DELIVERY_IND:
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

#endif /* PRIVATE_HH */
