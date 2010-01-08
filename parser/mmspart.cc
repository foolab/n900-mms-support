#include "mmspart.hh"
#include "qwsppdu.h"
#include <QBuffer>
#include <QDebug>

MmsPart::MmsPart(const QWspPart& part) : m_part(part) {
  const QList<QWspField> headers = part.headers();
  /*
    for (int x = 0; x < headers.size(); x++) {
    qDebug() << "Header" << headers.at(x).name << headers.at(x).value;
    }
  */
}

MmsPart::MmsPart(const QString& mimeType, const QByteArray& data, const QString& name) {
  QBuffer b(const_cast<QByteArray *>(&data));
  b.open(QIODevice::ReadOnly);
  m_part.readData(&b, data.size());

  m_part.addHeader("Content-Type", mimeType);
  if (!name.isEmpty()) {
    m_part.addHeader("Content-Location", name);
  }

}

QByteArray MmsPart::data() const {
  return m_part.data();
}

QString MmsPart::contentType() const {
  const QWspField *f = m_part.header("Content-Type");
  if (!f) {
    return QString();
  }

  QString type = f->value;

  int index = type.indexOf(';');

  if (index != -1) {
    type = type.left(index);
  }

  return type;
}

QString MmsPart::charset() const {
  const QWspField *f = m_part.header("Content-Type");
  if (!f) {
    return QString();
  }

  QString type = f->value;

  int index = type.indexOf("charset=");

  if (index == -1) {
    return QString();
  }

  index += 8;
  type = type.mid(index);

  index = type.indexOf(';');
  if (index != -1) {
    type = type.left(index);
  }

  index = type.indexOf('"');
  if (index != -1) {
    type = type.left(index);
  }

  return type;
}

bool MmsPart::isSmil() const {
  return contentType() == "application/smil";
}

bool MmsPart::isText() const {
  return contentType().startsWith("text/");
}

QString MmsPart::suggestedName() const {
  const QWspField *f = m_part.header("Content-Location");
  if (f) {
    return f->value;
  }

  f = m_part.header("Content-Type");
  if (!f) {
    return QString();
  }

  QString type = f->value;

  int index = type.indexOf("name=");
  if (index == -1) {
    return QString();
  }

  index += 5;
  type = type.mid(index);

  if (type.startsWith('"')) {
    type.remove(0, 1);
  }

  if (type.endsWith('"')) {
    type.chop(1);
  }

  return type;
}

void MmsPart::encode(QWspPduEncoder& e) const {
  e.encodePart(m_part);
}

QString MmsPart::contentId() const {
  const QWspField *f = m_part.header("Content-ID");
  if (f) {
    return f->value;
  }

  return QString();
}
