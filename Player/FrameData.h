/**
 * @file FrameData.h
 * @author Mikra Selene
 * @version OK
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

#ifndef EUTERPE_PLAYER_FRAMEDATA_H_
#define EUTERPE_PLAYER_FRAMEDATA_H_

#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QPointer>
#include <QVideoFrame>
#include <QVideoSink>

#include "../UI/ui_Player.h"
#include "Player.h"

#define nd [[nodiscard]]
#define mu [[maybe_unused]]

class FrameData : public QObject {
  Q_OBJECT
 public:
  explicit FrameData(const QPointer<Player> &player);
  ~FrameData() override = default;

  nd mu auto getFrame() const -> QImage;
  void targetFrameImage(const qint64 &now);
  void setIsVideo(bool isVideo);
  nd auto getTargetFrameTime(const qint64 &now, const qint64 &add) const
      -> qint64;

 private:
  void reloadFrameData(const QVideoFrame &nowFrame);
  void resetFrameList(const QUrl &url);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
 signals:
  void getFrameDone(const QImage &image);
#pragma clang diagnostic pop

 private:
  bool done_;
  bool isVideo_;
  qint64 timeRange_;
  QImage image_;
  QPointer<QVideoSink> sink_;
  QPointer<QMediaPlayer> mediaPlayer_;
};

#undef mu
#undef nd

#endif  // EUTERPE_PLAYER_FRAMEDATA_H_
