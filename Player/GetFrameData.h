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

#ifndef GETFRAMEDATA_H
#define GETFRAMEDATA_H
#include <QMediaPlayer>
#include <QPointer>
#include <QVideoFrame>
#include <QVideoSink>

#include "../UI/ui_Player.h"
#include "Player.h"

class GetFrameData : public QObject {
  Q_OBJECT
 private:
  QPointer<QVideoSink> sink;
  QPointer<QMediaPlayer> media_player;
  qint64 timeRange;  //视频帧间隔
  QImage image;
  bool done;
  bool isVideo;
  void ReloadFrameData(const QVideoFrame &nowFrame);
  int GetNowFrameNum(qint64 time);
  void resetFrameList(const QUrl &);
 signals:
  //在controller部分接收信号
  void doneGetFrame(QImage image);

 public:
  GetFrameData(const QPointer<Player> &player);
  void GetTargetFrameImage(qint64 now);
  QImage GetFrame();
  // now指当前时间，add指添加帧数
  qint64 GetTargetFrameTime(qint64 now, int add);
  void setIsVideo(bool);
};

#endif  // GETFRAMEDATA_H
