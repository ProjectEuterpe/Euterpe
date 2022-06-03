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

#include "MediaList.h"

MediaList::MediaList(QPointer<Player> parent_) {
  player_ = parent_;
  mediaPlayer_ = QPointer<QMediaPlayer>{new QMediaPlayer()};
  currIndex = -1;
  connect(mediaPlayer_, &QMediaPlayer::metaDataChanged, this,
          &MediaList::onChangeMetaData);
}

void MediaList::addMediaItemBox(QPointer<MediaItemBox> mediaItemBox) {
  mediaList_.append(mediaItemBox);
  player_->addMediaItemBox(mediaItemBox);
  connect(mediaItemBox, &MediaItemBox::play, this, &MediaList::checkCurrMedia);
  connect(mediaItemBox, &MediaItemBox::pause, this, &MediaList::pause);
  connect(mediaItemBox, &MediaItemBox::stop, this, &MediaList::stop);
  connect(mediaItemBox, &MediaItemBox::deleteMedia, this,
          &MediaList::deleteMedia);
}

void MediaList::onChangeMetaData() {
  qDebug() << "onChangeMetaData";
  QMediaMetaData metaData = mediaPlayer_->metaData();

  if (metaData.isEmpty()) return;

  MediaItemBox* widget = new MediaItemBox(player_);
  QString artist = metaData.value(QMediaMetaData::Author).toString();
  QString title = metaData.value(QMediaMetaData::Title).toString();
  widget->setMetaData(metaData);
  widget->setArtist(artist);
  widget->setTitle(title);
  widget->setMediaUrl(mediaPlayer_->source());

  if (!mediaList_.length()) {
    currMediaItem_ = widget;
    currIndex = 0;
    currMediaItem_->setActive(true);
  }
  addMediaItemBox(widget);
}

void MediaList::importMedia(const QUrl& url) {
  for (const auto& item : mediaList_) {
    if (item->getMediaUrl() == url) return;
  }
  mediaPlayer_->setSource(url);
}

void MediaList::playCurrMedia() { currMediaItem_->setBtnPlay(true); }

void MediaList::stopCurrMedia() { currMediaItem_->setBtnPlay(false); }

int MediaList::findMediaItem(QUrl url) {
  for (int i = 0, len = mediaList_.length(); i < len; i++) {
    if (mediaList_[i]->getMediaUrl() == url) {
      return i;
    }
  }
  return -1;
}

void MediaList::checkCurrMedia(QUrl url) {
  if (url == currMediaItem_->getMediaUrl()) {
    qDebug() << "play" << url;
    emit play();
    return;
  }
  int index = findMediaItem(url);
  if (index >= 0) {
    qDebug() << index;
    currMediaItem_->setActive(false);
    currMediaItem_ = mediaList_[index];
    currIndex = index;
    currMediaItem_->setActive(true);
    emit changeCurrMedia(url);
  }
}

void MediaList::deleteMedia(QUrl url) {
  int index = findMediaItem(url);
  if (index >= 0) {
    player_->deleteMediaItemBox(mediaList_[index]);
    mediaList_.removeAt(index);
    if (index == currIndex) {
      onNextMedia();
    }
  }
  if(index==0) emit endMedialist();
}

void MediaList::onNextMedia() {
  switch (playOrder_) {
    case PlayOrder::OnlyOnce: {
      emit stop();
      break;
    };
    case PlayOrder::InOrder: {
      currIndex = (currIndex + 1) % mediaList_.length();
      currMediaItem_->setActive(false);
      currMediaItem_ = mediaList_[currIndex];
      currMediaItem_->setActive(true);
      currMediaItem_->setBtnPlay(false);
      emit changeCurrMedia(currMediaItem_->getMediaUrl());
      break;
    };
    case PlayOrder::Random: {
      currIndex = (currIndex + int(rand())) % mediaList_.length();
      currMediaItem_->setActive(false);
      currMediaItem_ = mediaList_[currIndex];
      currMediaItem_->setActive(true);
      currMediaItem_->setBtnPlay(false);
      emit changeCurrMedia(currMediaItem_->getMediaUrl());
      break;
    };
    case PlayOrder::SingleLoop: {
      emit play();
      break;
    };
  }
  bool continuePlay = playOrder_ != PlayOrder::OnlyOnce;
  player_->setButtonPlayIcon(!continuePlay);
}

void MediaList::onChangePlayOrder() {
  switch (playOrder_) {
    case PlayOrder::OnlyOnce: {
      qDebug() << "clicked: btn_play_order change to play onlyOnce";
      playOrder_ = PlayOrder::InOrder;
      player_->setPlayOrderIcon(PlayOrder::InOrder);
      break;
    };
    case PlayOrder::InOrder: {
      qDebug() << "clicked: btn_play_order change to play randomLoop";
      playOrder_ = PlayOrder::Random;
      player_->setPlayOrderIcon(PlayOrder::Random);
      break;
    };
    case PlayOrder::Random: {
      qDebug() << "clicked: btn_play_order change to play singleLoop";
      playOrder_ = PlayOrder::SingleLoop;
      player_->setPlayOrderIcon(PlayOrder::SingleLoop);
      break;
    };
    case PlayOrder::SingleLoop: {
      qDebug() << "clicked: btn_play_order change to play onlyOnce";
      playOrder_ = PlayOrder::OnlyOnce;
      player_->setPlayOrderIcon(PlayOrder::OnlyOnce);
      break;
    };
    default: {
      qDebug() << "error!!";
    };
  }
}

void MediaList::playPrevMedia() {
    qDebug() << "clicked: playPrevMedia";
    if(currIndex - 1 == -1) {
        currIndex = mediaList_.length()-1;
    }
    else {
        currIndex = (currIndex - 1) % mediaList_.length();
    }
    currMediaItem_->setActive(false);
    currMediaItem_ = mediaList_[currIndex];
    currMediaItem_->setActive(true);
    emit changeCurrMedia(currMediaItem_->getMediaUrl());
}

void MediaList::playNextMedia() {
    qDebug() << "clicked: playNextMedia";
    currIndex = (currIndex + 1) % mediaList_.length();
    currMediaItem_->setActive(false);
    currMediaItem_ = mediaList_[currIndex];
    currMediaItem_->setActive(true);
    emit changeCurrMedia(currMediaItem_->getMediaUrl());
}

void MediaList::initDatabase() {}
