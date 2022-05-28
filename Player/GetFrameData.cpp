/*
 * @file
 * @author Mikra Selene
 * @version
 * @date
 *
 * @section LICENSE
 *
 * Copyright (C) 2022 Mikra Selene
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "GetFrameData.h"

#include <QMediaMetaData>

void GetFrameData::resetFrameList(const QUrl &media) {
  media_player->setSource(media);
  timeRange = -1;
  done = false;
  isVideo = false;
  media_player->play();
}

GetFrameData::GetFrameData(const QPointer<Player> &player) {
  media_player = QPointer<QMediaPlayer>{new QMediaPlayer()};
  //初始化sink；
  sink = QPointer<QVideoSink>{new QVideoSink()};
  media_player->setVideoSink(sink);
  connect(player->mediaPlayer(), &QMediaPlayer::sourceChanged, this,
          &GetFrameData::resetFrameList);
  connect(sink, &QVideoSink::videoFrameChanged, this,
          &GetFrameData::ReloadFrameData);
}

void GetFrameData::ReloadFrameData(const QVideoFrame &nowFrame) {  //播放器停止
  if (!isVideo || done) return;
  media_player->stop();
  if (timeRange == -1)  //获取第一帧以获取帧的时间间隔,更换视频url时会调用
  {
    timeRange = (nowFrame.endTime() - nowFrame.startTime()) / 1000;
    qDebug() << "TimeRange:" << timeRange;
  } else  //获取帧图像
  {
    image = nowFrame.toImage();
    emit doneGetFrame(image);
  }
  done = true;
}

void GetFrameData::GetTargetFrameImage(qint64 now) {
  done = false;
  media_player->setPosition(now);
  media_player->play();
}
QImage GetFrameData::GetFrame() { return image; }

qint64 GetFrameData::GetTargetFrameTime(qint64 now, int add) {
  int targetFrame = (now) / timeRange + add;
  return timeRange * targetFrame;
}

void GetFrameData::setIsVideo(bool is) { isVideo = is; }
