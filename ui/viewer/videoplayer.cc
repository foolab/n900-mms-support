#include "videoplayer.hh"
#include <Phonon/VideoPlayer>
#include <Phonon/MediaObject>
#include <Phonon/MediaSource>
#include <Phonon/SeekSlider>
#include <Phonon/VolumeSlider>
#include <QBuffer>
#include <QGridLayout>
#include <QPushButton>

// TODO: error.

VideoPlayer::VideoPlayer(const QByteArray& data, bool hasVideo, QWidget *parent) : QWidget(parent) {
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  Phonon::VideoPlayer *player = new Phonon::VideoPlayer(this);
  layout->addWidget(player, 0, 0, 1, 5);

  player->setVisible(hasVideo);

  QBuffer *buffer = new QBuffer(this);
  buffer->setData(data);
  Phonon::MediaSource src(buffer);
  src.setAutoDelete(true);

  m_mediaObject = player->mediaObject();
  m_mediaObject->setQueue(QList<Phonon::MediaSource>() << src);

  QObject::connect(m_mediaObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(stateChanged()));

  m_stop = new QPushButton(tr("&Stop"), this);
  m_play = new QPushButton(tr("&Play"), this);
  m_pause = new QPushButton(tr("Pa&use"), this);

  QObject::connect(m_stop, SIGNAL(clicked()), m_mediaObject, SLOT(stop()));
  QObject::connect(m_play, SIGNAL(clicked()), m_mediaObject, SLOT(play()));
  QObject::connect(m_pause, SIGNAL(clicked()), m_mediaObject, SLOT(pause()));

  layout->addWidget(m_stop, 2, 0, 1, 1);
  layout->addWidget(m_play, 2, 1, 1, 1);
  layout->addWidget(m_pause, 2, 2, 1, 1);

  Phonon::SeekSlider *slider = new Phonon::SeekSlider(this);
  slider->setMediaObject(m_mediaObject);

  layout->addWidget(slider, 1, 0, 1, 5);

  Phonon::VolumeSlider *volume = new Phonon::VolumeSlider(this);
  volume->setOrientation(Qt::Horizontal);
  layout->addWidget(volume, 2, 3, 1, 2);
  volume->setAudioOutput(player->audioOutput());
  QObject::connect(m_mediaObject, SIGNAL(finished()), this, SLOT(finished()));

  stateChanged();
}

VideoPlayer::~VideoPlayer() {

}

void VideoPlayer::stateChanged() {
  switch (m_mediaObject->state()) {
  case Phonon::LoadingState:
    m_play->setEnabled(false);
    m_pause->setEnabled(false);
    m_stop->setEnabled(false);
    break;
  case Phonon::StoppedState:
    m_play->setEnabled(true);
    m_pause->setEnabled(false);
    m_stop->setEnabled(false);
    break;
  case Phonon::PlayingState:
    m_play->setEnabled(false);
    m_pause->setEnabled(true);
    m_stop->setEnabled(true);
    break;
  case Phonon::BufferingState:
    m_play->setEnabled(false);
    m_pause->setEnabled(false);
    m_stop->setEnabled(false);
    break;
  case Phonon::PausedState:
    m_play->setEnabled(true);
    m_pause->setEnabled(false);
    m_stop->setEnabled(true);
    break;
  case Phonon::ErrorState:
    m_play->setEnabled(false);
    m_pause->setEnabled(false);
    m_stop->setEnabled(false);
    break;
  }
}

void VideoPlayer::finished() {
  m_mediaObject->stop();
  stateChanged();
}
