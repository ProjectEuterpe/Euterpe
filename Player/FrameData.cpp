/**
 * @file FrameData.cpp
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

#include "FrameData.h"

FrameData::FrameData(const QPointer<Player> &player)
    : done_(false),
      isVideo_(false),
      timeRange_(-1),
      sink_(QPointer<QVideoSink>(new QVideoSink())),
      mediaPlayer_(QPointer<QMediaPlayer>(new QMediaPlayer())) {
  this->mediaPlayer_->setVideoSink(this->sink_);
  connect(player->mediaPlayer(), &QMediaPlayer::sourceChanged, this,
          &FrameData::resetFrameList);
  connect(this->sink_, &QVideoSink::videoFrameChanged, this,
          &FrameData::reloadFrameData);
}

[[maybe_unused]] auto FrameData::getFrame() const -> QImage {
  return this->image_;
}

void FrameData::targetFrameImage(const qint64 &now) {
  this->done_ = false;
  this->mediaPlayer_->setPosition(now);
  this->mediaPlayer_->play();
}

void FrameData::setIsVideo(bool isVideo) { this->isVideo_ = isVideo; }

auto FrameData::getTargetFrameTime(const qint64 &now, const qint64 &add) const
    -> qint64 {
  auto targetFrame = now / this->timeRange_ + add;
  return this->timeRange_ * targetFrame;
}

void FrameData::reloadFrameData(const QVideoFrame &nowFrame) {
  if (!this->isVideo_ || this->done_) {
    return;
  }
  this->mediaPlayer_->stop();
  if (this->timeRange_ == -1) {
    this->timeRange_ = (nowFrame.endTime() - nowFrame.startTime()) / 1000;
  } else {
    this->image_ = nowFrame.toImage();
    emit this->getFrameDone(this->image_);
  }
  this->done_ = true;
}

void FrameData::resetFrameList(const QUrl &url) {
  this->done_ = false;
  this->isVideo_ = false;
  this->timeRange_ = -1;
  this->mediaPlayer_->setSource(url);
  this->mediaPlayer_->play();
}
