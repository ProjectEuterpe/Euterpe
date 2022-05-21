#ifndef GETFRAMEDATA_H
#define GETFRAMEDATA_H
#include "Player.h"
#include "ui_Player.h"
#include<QVideoSink>
#include<QPointer>
#include<QMediaPlayer>
#include<QVideoFrame>
class GetFrameData:public QObject
{Q_OBJECT
private:
   QPointer<QVideoSink>  sink;
QPointer<QMediaPlayer>media_player;
 qint64 timeRange;//视频帧间隔
QImage image;
bool done;
bool isVideo;
void ReloadFrameData(const QVideoFrame &nowFrame);
  int GetNowFrameNum(qint64 time);
  void resetFrameList(const QUrl &);
signals :
  //在controller部分接收信号
  void doneGetFrame(QImage image);
public:
    GetFrameData(const QPointer<Player> &player);
   void GetTargetFrameImage(qint64 now);
  QImage GetFrame();
  //now指当前时间，add指添加帧数
  qint64 GetTargetFrameTime(qint64 now,int add);
 void setIsVideo(bool);
};

#endif // GETFRAMEDATA_H
