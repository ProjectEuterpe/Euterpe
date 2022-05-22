#ifndef MEDIALIST_H
#define MEDIALIST_H

#include "MediaItemBox.h"
#include "GetFrameData.h"

class MediaList:public QObject
{
  Q_OBJECT
private:
  QPointer<Player> player_;
  QPointer<QMediaPlayer> media_player_;
  QList<QPointer<MediaItemBox>> mediaList;
  QPointer<MediaItemBox> currMediaItem;
  int currIndex;
  enum PlayOrder { onlyOnce = 1, inOrder, randomLoop, singleLoop} playOrder = onlyOnce;
  void checkCurrMedia(QUrl url);

public:
  MediaList(QPointer<Player> player_);
  int findMediaItem(QUrl url);
  void addMediaItemBox(QPointer<MediaItemBox> mediaItemBox);
  void addMediaItem(QUrl url);
  void playCurrMedia();
  void stopCurrMedia();

signals:
  play();
  pause();
  stop();
  changeCurrMedia(QUrl url);

private slots:
  void onChangeMetaData();
  void deleteMedia(QUrl url);

public slots:
  void onChangePlayOrder();
  void onNextMedia();

};

#endif // MEDIALIST_H
