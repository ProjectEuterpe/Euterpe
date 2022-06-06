/**
 * @file MediaList.cpp
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

#include "MediaList.h"

#pragma region public

/**
 * @brief
 * @param player
 */
MediaList::MediaList(const QPointer<Player>& player)
    : player_(player), playOrder_(PlayOrder::SingleLoop), currentIndex_(0) {
  this->mediaPlayer_ = QPointer<QMediaPlayer>(new QMediaPlayer());
  connect(this->mediaPlayer_, &QMediaPlayer::metaDataChanged, this,
          &MediaList::onChangeMetaData);
  this->initDatabase();
}

/**
 * @brief Get the database table.
 * @return
 */
auto MediaList::databaseTable() const -> QList<QSharedPointer<MediaData>> {
  return this->database_->table();
}

/**
 * @brief Insert a media to database.
 * @param url
 */
void MediaList::insertToDatabase(const QUrl& url) const {
  auto rows = this->database_->find(MediaDataEnum::MEDIA_PATH, url.path());
  if (rows.empty()) {
    // if media does not exist, insert it.
    auto md5 = [=]() -> QString {
      auto mediaFile = QFile(url.path());
      mediaFile.open(QIODevice::ReadOnly);
      auto md5 = QString(
          QCryptographicHash::hash(mediaFile.readAll(), QCryptographicHash::Md5)
              .toHex());
      mediaFile.close();
      return md5;
    }();
    auto mediaName = url.fileName();
    auto mediaPath = url.path();
    auto label = QString();
    auto metadata = MetaData(this->player_->metaData()).toJsonStringCompact();
    auto timestamp = QDateTime::currentDateTime().toSecsSinceEpoch();
    this->database_->insert(QSharedPointer<MediaData>(new MediaData{
        md5, mediaName, mediaPath, label, metadata, timestamp, timestamp}));
  } else {
    // if media exists, update it.
    for (const auto& row : rows) {
      auto timestamp = QDateTime::currentDateTime().toSecsSinceEpoch();
      this->database_->update(row, MediaDataEnum::PLAY_TIMESTAMP, timestamp);
    }
  }
}

/**
 * @brief Find a media in the database.
 * @param url
 * @return
 */
auto MediaList::findInDatabase(const QUrl& url) const
    -> QList<QSharedPointer<MediaData>> {
  return this->database_->find(MediaDataEnum::MEDIA_PATH, url.path());
}

/**
 * @brief Add a media item box.
 * @param url
 * @param artist
 * @param title
 */
void MediaList::addMediaItemBox(const QUrl& url, const QString& artist,
                                const QString& title) {
  auto mediaItemBox = new MediaItemBox(this->player_);
  auto metaData = mediaPlayer_->metaData();
  mediaItemBox->setMetaData(metaData);
  mediaItemBox->setMediaUrl(url);
  mediaItemBox->setMediaArtist(artist);
  mediaItemBox->setMediaTitle(title);
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
  mediaItemBoxConnection(removeMedia, onRemoveMedia);
#undef mediaItemBoxConnection
}

/**
 * @brief Import media.
 * @param url
 */
void MediaList::importMedia(const QUrl& url) {
  qDebug() << "IMPORT" << url;
  auto check = QFileInfo(url.path());
  if (check.exists() && check.isFile()) {
    this->mediaPlayer_->setSource(url);
    this->checkCurrentMedia(url);
  } else {
    // TODO: Media file problem!
  }
}

/**
 * @brief To play, or not to play, that is the question.
 * @param play
 */
void MediaList::playStop(bool play) const {
  this->currentMediaItem_->setButtonPlay(play);
}

/**
 * @brief To play prev media or to play next media, that is also the question.
 * @param seq
 */
void MediaList::playPrevNextMedia(const Sequence& seq) {
  switch (this->playOrder_) {
    case PlayOrder::OnlyOnce:
    case PlayOrder::SingleLoop:
    case PlayOrder::InOrder: {
      this->stepForward(seq == Sequence::Prev ? -1 : +1);
      return;
    }
    case PlayOrder::Random: {
      auto rd = std::random_device();
      auto mt = std::mt19937(rd());
      auto dist = std::uniform_int_distribution<qsizetype>(
          1, this->mediaList_.length() - 1);
      this->stepForward(dist(mt));
      return;
    }
  }
}

#pragma endregion

#pragma region private

/**
 * @brief Initialize database.
 */
void MediaList::initDatabase() {
  auto connection = QSharedPointer<ConnectionArgs>(new ConnectionArgs{
      "QSQLITE", QApplication::applicationDirPath() + "/database2.dat",
      "selene", "123456", "localhost", 30000});
  this->database_ = QSharedPointer<MediaListSql>(new MediaListSql(connection));
  this->database_->connect();
}

/**
 * @brief Step forward.
 * @param step
 */
void MediaList::stepForward(const qint64& step) {
  this->currentIndex_ = [](qint64 x, qint64 n) -> qint64 {
    return (x % n + n) % n;  // to avoid negative index.
  }(this->currentIndex_ + step, this->mediaList_.length());
  this->currentMediaItem_->setActive(false);
  this->currentMediaItem_ = this->mediaList_[this->currentIndex_];
  this->currentMediaItem_->setActive(true);
  currentMediaItem_->setButtonPlay(false);
  emit this->changeCurrentMedia(this->currentMediaItem_->getMediaUrl());
}

/**
 * @brief Find a media by media url in the media list.
 * @param url: Given url.
 * @return A QList iterator.
 */
auto MediaList::findMedia(const QUrl& url)
    -> QList<QPointer<MediaItemBox>>::iterator {
  return std::find_if(this->mediaList_.begin(), this->mediaList_.end(),
                      [=](auto i) { return i->getMediaUrl() == url; });
}

/**
 * @brief The inspection of the media is necessary when playing.
 * @param url: Given url.
 */
void MediaList::checkCurrentMedia(const QUrl& url) {
  if (url == this->currentMediaItem_->getMediaUrl()) {
    // if media is not changed...
    emit this->play(this->currentMediaItem_->getMediaUrl());
  } else {
    // change the media.
    auto iterator = this->findMedia(url);
    if (iterator != this->mediaList_.end()) {
      this->currentMediaItem_->setActive(false);
      this->currentMediaItem_ = *iterator;
      this->currentIndex_ = std::distance(this->mediaList_.begin(), iterator);
      this->currentMediaItem_->setActive(true);
      emit changeCurrentMedia(url);
      qDebug() << "index changed: current index =" << this->currentIndex_;
    } else {
      // TODO: ILLEGAL URL!
      return;
    }
  }
  auto rows = this->database_->find(MediaDataEnum::MEDIA_PATH, url.path());
  for (const auto& row : rows) {
    // update the play timestamp in the database.
    auto timestamp = QDateTime::currentDateTime().toSecsSinceEpoch();
    this->database_->update(row, MediaDataEnum::PLAY_TIMESTAMP, timestamp);
  }
}

#pragma endregion

#pragma region slots

/**
 * @brief Play next media handler.
 */
void MediaList::onNextMedia() {
  switch (this->playOrder_) {
    case PlayOrder::OnlyOnce: {
      emit stop();
      break;
    }
    case PlayOrder::SingleLoop: {
      emit play(this->currentMediaItem_->getMediaUrl());
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

/**
 * @brief Change play order handler.
 */
void MediaList::onChangePlayOrder() {
  auto changePlayOrder = [&](PlayOrder playOrder) {
    this->playOrder_ = playOrder;
    this->player_->setPlayOrderIcon(playOrder);
  };
  qDebug() << "play order changed:";
  switch (this->playOrder_) {
    case PlayOrder::OnlyOnce: {
      qDebug() << "current play order is InOrder";
      changePlayOrder(PlayOrder::InOrder);
      return;
    }
    case PlayOrder::InOrder: {
      qDebug() << "current play order is RandomLoop";
      changePlayOrder(PlayOrder::Random);
      return;
    }
    case PlayOrder::Random: {
      qDebug() << "current play order is SingleLoop";
      changePlayOrder(PlayOrder::SingleLoop);
      return;
    }
    case PlayOrder::SingleLoop: {
      qDebug() << "current play order is OnlyOnce";
      changePlayOrder(PlayOrder::OnlyOnce);
      return;
    }
  }
}

/**
 * @brief When removing the media.
 * @param url
 */
void MediaList::onRemoveMedia(const QUrl& url) {
  qDebug() << "ON REMOVE!" << url;
  auto iterator = this->findMedia(url);
  if (iterator != this->mediaList_.end()) {
    this->player_->deleteMediaItemBox(*iterator);  // TODO: Change to QPointer?
    this->mediaList_.erase(iterator);
    auto index = std::distance(this->mediaList_.begin(), iterator);
    if (index == this->currentIndex_) {
      // if remove the current playing media, play next media.
      this->onNextMedia();
    }
    // remove media from the database.
    auto rows = this->database_->find(MediaDataEnum::MEDIA_PATH, url.path());
    qDebug() << rows;
    for (const auto& row : rows) {
      this->database_->remove(row);
    }
  } else {
    emit this->endOfMediaList();
  }
}

/**
 * @brief Change meta data handler.
 */
void MediaList::onChangeMetaData() {
  auto metaData = this->mediaPlayer_->metaData();
  auto artist = metaData.value(QMediaMetaData::AlbumTitle).toString();
  auto title = metaData.value(QMediaMetaData::Title).toString();
  if (!metaData.isEmpty()) {
    this->addMediaItemBox(this->mediaPlayer_->source(), artist, title);
  }
}

#pragma endregion
