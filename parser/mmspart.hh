#ifndef MMS_PART_HH
#define MMS_PART_HH

#include "qwsppdu.h"

class QWspPduEncoder;

class MmsPart {
public:
  MmsPart(const QWspPart& part);
  MmsPart(const QString& mimeType, const QByteArray& data, const QString& name = QString());

  QByteArray data() const;

  QString contentType() const;
  QString charset() const;
  QString suggestedName() const;
  QString contentId() const;

  bool isSmil() const;
  bool isText() const;
  bool isVideo() const;
  bool isAudio() const;
  bool isImage() const;

  void encode(QWspPduEncoder& e) const;

private:
  QWspPart m_part;
};

#endif /* MMS_PART_HH */
