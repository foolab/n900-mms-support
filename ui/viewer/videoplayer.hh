#ifndef VIDEO_PLAYER_HH
#define VIDEO_PLAYER_HH

#include <QWidget>

namespace Phonon { class MediaObject; }
class QPushButton;

class VideoPlayer : public QWidget {
  Q_OBJECT

public:
  VideoPlayer(const QByteArray& data, bool hasVideo, QWidget *parent = 0);
  ~VideoPlayer();

private slots:
  void stateChanged();
  void finished();

private:
  Phonon::MediaObject *m_mediaObject;

  QPushButton *m_stop;
  QPushButton *m_play;
  QPushButton *m_pause;
};

#endif /* VIDEO_PLAYER_HH */
