#include "GetFrameData.h"
#include "ui_Player.h"
void GetFrameData::resetFrameList(const QUrl & media)
{
media_player->setSource(media);
timeRange=-1;
media_player->play();
}

GetFrameData::GetFrameData(const QPointer<Player> &player)
{
    media_player=QPointer<QMediaPlayer>{new QMediaPlayer()};
//初始化sink；
sink=QPointer<QVideoSink>{new QVideoSink()};
media_player->setVideoSink(sink);
connect(player->mediaPlayer(),&QMediaPlayer::sourceChanged,this,&GetFrameData::resetFrameList);
 connect(sink, &QVideoSink::videoFrameChanged, this, &GetFrameData::ReloadFrameData);
}

void GetFrameData::ReloadFrameData(const QVideoFrame &nowFrame)
{
  if(timeRange==-1)//获取第一帧以获取帧的时间间隔,更换视频url时会调用
  {
      timeRange=(nowFrame.endTime()-nowFrame.startTime())/1000;
      qDebug()<<"TimeRange:"<<timeRange;
  }
  else//获取帧图像
  {
   image=nowFrame.toImage();
  }
    //播放器停止
  media_player->stop();
  emit doneGetFrame(image);
}

void  GetFrameData::GetTargetFrameImage(qint64 now)
{

    media_player->setPosition(now);
    media_player->play();
}
QImage GetFrameData::GetFrame(){
    return image;
}

qint64 GetFrameData::GetTargetFrameTime(qint64 now, int add)
{
    //当前帧num
 int targetFrame=(now)/timeRange+add;
 return  timeRange*targetFrame;
}






