#include "smilviewer.hh"
#include "mmsmessage.hh"
#include <QFile>
#include <QLabel>
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include "videoplayer.hh"
#include <QDebug>
#include <QScrollArea>
#include <QDomDocument>

SmilViewer::SmilViewer(const QString& path, QMainWindow *parent) : QMainWindow(parent) {
  QScrollArea *area = new QScrollArea(this);
  setCentralWidget(area);

  QWidget *widget = new QWidget(this);

  QFile file(path);
  file.open(QFile::ReadOnly);
  m_message = new MmsMessage(&file);

  m_layout = new QGridLayout(widget);
  m_layout->setContentsMargins(0, 0, 0, 0);

  m_layout->addWidget(new QLabel(tr("From:"), this), 0, 0, 1, 1);
  m_layout->addWidget(new QLabel(m_message->from(), this), 0, 1, 1, 1);
  m_layout->addWidget(new QLabel(tr("Subject:"), this), 1, 0, 1, 1);
  m_layout->addWidget(new QLabel(m_message->subject(), this), 1, 1, 1, 1);

  QList<MmsPart> parts = m_message->parts();

  if (parts.isEmpty()) {
    return;
  }

  int smilPart = -1;
  for (int x = 0; x < parts.size(); x++) {
    if (parts.at(x).isSmil()) {
      smilPart = x;
      break;
    }
  }

  if (smilPart == -1) {
    displayAll(parts);
    return;
  }

  MmsPart smil = parts.takeAt(smilPart);

  qDebug() << smil.data();

  QDomDocument doc;
  if (!doc.setContent(smil.data(), false)) {
    displayAll(parts);
    return;
  }

  int nextRow = 2;
  QMap<QString, int> map;
  for (int x = 0; x < parts.size(); x++) {
    map[parts.at(x).suggestedName()] = x;
  }

  QDomNodeList pars = doc.elementsByTagName("par");
  for (int x = 0; x < pars.size(); x++) {
    QDomElement par = pars.at(x).toElement();
    if (par.isNull()) {
      continue;
    }

    for (QDomNode n = par.firstChild(); !n.isNull(); n = n.nextSibling()) {
      QDomElement elem = n.toElement();
      QString tag = elem.tagName();
      QString src = elem.attribute("src");
      if (!map.contains(src)) {
	continue;
      }
      else if (tag == "img") {
	m_layout->addWidget(imageWidget(parts.at(map[src])), nextRow++, 0, 1, 2);
      }
      else if (tag == "text") {
	m_layout->addWidget(textWidget(parts.at(map[src])), nextRow++, 0, 1, 2);
      }
      else if (tag == "video") {
	m_layout->addWidget(videoWidget(parts.at(map[src])), nextRow++, 0, 1, 2);
      }
      else if (tag == "audio") {
	m_layout->addWidget(audioWidget(parts.at(map[src])), nextRow++, 0, 1, 2);
      }
      else {
	m_layout->addWidget(unknownWidget(parts.at(map[src])), nextRow++, 0, 1, 2);
      }
    }
  }

  //  while (!par.isNull()) {

  //  }

  area->setWidget(widget);
}

SmilViewer::~SmilViewer() {
  delete m_message;
}

void SmilViewer::displayAll(const QList<MmsPart>& parts) {
  int nextRow = 2;

  // TODO: charset conversion

  for (int x = 0; x < parts.size(); x++) {
    if (parts.at(x).isText()) {
      m_layout->addWidget(textWidget(parts.at(x)), nextRow++, 0, 1, 2);
    }
    else if (parts.at(x).isImage()) {
      m_layout->addWidget(imageWidget(parts.at(x)), nextRow++, 0, 1, 2);
    }
    else if (parts.at(x).isVideo()) {
      m_layout->addWidget(videoWidget(parts.at(x)), nextRow++, 0, 1, 2);
    }
    else if (parts.at(x).isAudio()) {
      m_layout->addWidget(audioWidget(parts.at(x)), nextRow++, 0, 1, 2);
    }
    else {
      m_layout->addWidget(unknownWidget(parts.at(x)), nextRow++, 0, 1, 2);
    }
  }
}

QWidget *SmilViewer::imageWidget(const MmsPart& part) {
  QImage image = QImage::fromData(part.data());
  QLabel *label = new QLabel(this);
  label->setPixmap(QPixmap::fromImage(image));

  return label;
}

QWidget *SmilViewer::textWidget(const MmsPart& part) {
  return new QLabel(part.data(), this);
}

QWidget *SmilViewer::audioWidget(const MmsPart& part) {
  return new VideoPlayer(part.data(), false, this);
}

QWidget *SmilViewer::videoWidget(const MmsPart& part) {
  return new VideoPlayer(part.data(), true, this);
}

QWidget *SmilViewer::unknownWidget(const MmsPart& part) {
  return new QLabel(tr("Unknown object: %1").arg(part.contentType()), this);
}
