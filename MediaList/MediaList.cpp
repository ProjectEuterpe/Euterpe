/**
 * @file MediaList.cpp
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

MediaList::MediaList(const QPointer<Player>& parent)
    : player_(parent), playOrder_(PlayOrder::SingleLoop), currentIndex_(-1) {
  this->mediaPlayer_ = QPointer<QMediaPlayer>(new QMediaPlayer());
  connect(this->mediaPlayer_, &QMediaPlayer::metaDataChanged, this,
          &MediaList::onChangeMetaData);
  this->initDatabase();
}

void MediaList::addMediaItemBox(const QUrl& url, const QString& author,
                                const QString& title) {
  qDebug() << "addMediaItemBox";
  auto mediaItemBox = new MediaItemBox(this->player_);
  auto metaData = mediaPlayer_->metaData();
  mediaItemBox->setMetaData(metaData);
  mediaItemBox->setMediaArtist(author);
  mediaItemBox->setMediaTitle(title);
  mediaItemBox->setMediaUrl(url);
  if (!this->mediaList_.length()) {
    this->currentMediaItem_ = mediaItemBox;
    this->currentIndex_ = 0;
    this->currentMediaItem_->setActive(true);
  }
  this->mediaList_.append(mediaItemBox);
  this->player_->addMediaItemBox(mediaItemBox);
#define mediaItemBoxConnection(signal, slot) \
  connect(mediaItemBox, &MediaItemBox::signal, this, &MediaList::slot)
  mediaItemBoxConnection(play, checkCurrentMedia);
  mediaItemBoxConnection(pause, pause);
  mediaItemBoxConnection(stop, stop);
  mediaItemBoxConnection(deleteMedia, onRemoveMedia);
#undef mediaItemBoxConnection
}

void MediaList::onChangeMetaData() {  // ?
  qDebug() << "onChangeMetaData";
  auto metaData = this->mediaPlayer_->metaData();
  if (metaData.isEmpty()) {
    return;
  }
  QString artist = metaData.value(QMediaMetaData::AlbumTitle).toString();
  QString title = metaData.value(QMediaMetaData::Title).toString();
  this->addMediaItemBox(this->mediaPlayer_->source(), artist, title);
}

auto MediaList::databaseTable() -> QList<QSharedPointer<MediaData>> {
  return this->database_->table();
}

void MediaList::insertToDatabase(const QUrl& url) {
  auto rows = this->database_->find(MediaDataEnum::MEDIA_PATH, url.path());
  if (rows.empty()) {
    auto mediaFile = QFile(url.path());
    mediaFile.open(QIODevice::ReadOnly);
    auto md5 = QString(
        QCryptographicHash::hash(mediaFile.readAll(), QCryptographicHash::Md5)
            .toHex());
    auto mediaName = url.fileName();
    auto mediaPath = url.path();
    auto label = QString();
    auto metadata = MetaData(this->player_->metaData()).toJsonStringCompact();
    auto timestamp = QDateTime::currentDateTime().toSecsSinceEpoch();
    mediaFile.close();
    this->database_->insert(QSharedPointer<MediaData>(new MediaData{
        md5, mediaName, mediaPath, label, metadata, timestamp, timestamp}));
  } else {
    for (const auto& row : rows) {
      auto timestamp = QDateTime::currentDateTime().toSecsSinceEpoch();
      this->database_->update(row, MediaDataEnum::PLAY_TIMESTAMP, timestamp);
    }
  }
}

void MediaList::importMedia(const QUrl& url) {
  auto check = QFileInfo(url.path());
  if (check.exists() && check.isFile()) {
    this->mediaPlayer_->setSource(url);
  } else {
    qDebug() << "WARNING!";
  }
}

void MediaList::playStop(bool play) {
  this->currentMediaItem_->setButtonPlay(play);
}

void MediaList::playPrevMedia() {
  qDebug() << "clicked: playPrevMedia";
  switch (this->playOrder_) {
    case PlayOrder::OnlyOnce:
    case PlayOrder::SingleLoop:
    case PlayOrder::InOrder: {
      this->stepForward(-1);
      break;
    }
    case PlayOrder::Random: {
      auto rd = std::random_device();
      auto mt = std::mt19937(rd());
      auto dist = std::uniform_int_distribution<qsizetype>(
          1, this->mediaList_.length() - 1);
      this->stepForward(dist(mt));
      break;
    }
  }
}

void MediaList::playNextMedia() {
  qDebug() << "clicked: playNextMedia";
  switch (this->playOrder_) {
    case PlayOrder::OnlyOnce:
    case PlayOrder::SingleLoop:
    case PlayOrder::InOrder: {
      this->stepForward(+1);
      break;
    }
    case PlayOrder::Random: {
      auto rd = std::random_device();
      auto mt = std::mt19937(rd());
      auto dist = std::uniform_int_distribution<qsizetype>(
          1, this->mediaList_.length() - 1);
      this->stepForward(dist(mt));
      break;
    }
  }
}

void MediaList::stepForward(const qint64& step) {
  currentIndex_ = (currentIndex_ + step) % mediaList_.length();
  currentMediaItem_->setActive(false);
  currentMediaItem_ = mediaList_[currentIndex_];
  currentMediaItem_->setActive(true);
  emit changeCurrentMedia(currentMediaItem_->getMediaUrl());
}

auto MediaList::findMedia(const QUrl& url)
    -> QList<QPointer<MediaItemBox>>::iterator {
  return std::find_if(this->mediaList_.begin(), this->mediaList_.end(),
                      [=](auto i) { return i->getMediaUrl() == url; });
}

void MediaList::checkCurrentMedia(const QUrl& url) {
  if (url == this->currentMediaItem_->getMediaUrl()) {
    qDebug() << "play" << url;
    emit play();
    return;
  }
  auto iterator = findMedia(url);
  if (iterator != this->mediaList_.end()) {
    this->currentMediaItem_->setActive(false);
    this->currentMediaItem_ = *iterator;
    this->currentIndex_ = std::distance(iterator, this->mediaList_.begin());
    this->currentMediaItem_->setActive(true);
    emit changeCurrentMedia(url);
    qDebug() << "current index :" << this->currentIndex_;
  }
}

void MediaList::onRemoveMedia(const QUrl& url) {
  auto iterator = findMedia(url);
  if (iterator != this->mediaList_.end()) {
    this->player_->deleteMediaItemBox(*iterator);
    this->mediaList_.erase(iterator);
    auto index = std::distance(iterator, this->mediaList_.begin());
    if (index == this->currentIndex_) {
      onNextMedia();
    }
    auto rows = this->database_->find(MediaDataEnum::MEDIA_PATH, url.path());
    for (const auto& row : rows) {
      this->database_->remove(row);
    }
  } else {
    emit endOfMediaList();
  }
}

void MediaList::onNextMedia() {
  switch (this->playOrder_) {
    case PlayOrder::OnlyOnce: {
      emit stop();
      break;
    }
    case PlayOrder::SingleLoop: {
      emit play();
      break;
    }
    case PlayOrder::InOrder: {
      this->stepForward(+1);
      break;
    }
    case PlayOrder::Random: {
      auto rd = std::random_device();
      auto mt = std::mt19937(rd());
      auto dist = std::uniform_int_distribution<qsizetype>(
          1, this->mediaList_.length() - 1);
      this->stepForward(dist(mt));
      break;
    }
  }
  bool continuePlay = this->playOrder_ == PlayOrder::OnlyOnce;
  this->player_->setButtonPlayIcon(continuePlay);
}

void MediaList::onChangePlayOrder() {
  auto changePlayOrder = [&](PlayOrder playOrder) {
    this->playOrder_ = playOrder;
    this->player_->setPlayOrderIcon(playOrder);
  };
  switch (this->playOrder_) {
    case PlayOrder::OnlyOnce: {
      qDebug() << "play order changed: play onlyOnce";
      changePlayOrder(PlayOrder::InOrder);
      break;
    }
    case PlayOrder::InOrder: {
      qDebug() << "play order changed: play randomLoop";
      changePlayOrder(PlayOrder::Random);
      break;
    }
    case PlayOrder::Random: {
      qDebug() << "play order changed: play singleLoop";
      changePlayOrder(PlayOrder::SingleLoop);
      break;
    }
    case PlayOrder::SingleLoop: {
      qDebug() << "play order changed: play onlyOnce";
      changePlayOrder(PlayOrder::OnlyOnce);
      break;
    }
    default: {
      qDebug() << "?";
    }
  }
}

void MediaList::initDatabase() {
  auto connection = QSharedPointer<ConnectionArgs>(new ConnectionArgs{
      "QSQLITE", QApplication::applicationDirPath() + "/database.dat", "selene",
      "123456", "localhost", 400});
  this->database_ = QSharedPointer<MediaListSql>(new MediaListSql(connection));
  this->database_->connect();
  // get data from database...
  for (const auto& row : this->database_->table()) {
  }
}
