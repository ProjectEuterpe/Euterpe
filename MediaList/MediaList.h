/**
 * @file MediaList.h
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

#ifndef EUTERPE_MEDIALIST_MEDIALIST_H_
#define EUTERPE_MEDIALIST_MEDIALIST_H_

#include <QList>

#include "../MetaData/MetaData.h"
#include "../Player/GetFrameData.h"
#include "../Player/Player.h"
#include "MediaItemBox.h"
#include "MediaListSql.h"

class MediaList : public QObject {
  Q_OBJECT

 public:
  explicit MediaList(const QPointer<Player>& player_);
  ~MediaList() override = default;

  auto databaseTable() -> QList<QSharedPointer<MediaData>>;
  void insertToDatabase(const QUrl& url);
  void addMediaItemBox(const QUrl& url, const QString& author,
                       const QString& title);
  void importMedia(const QUrl& url);

  void playStop(bool play);

  void stepForward(const qint64& step);
  void playPrevMedia();
  void playNextMedia();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
 signals:
  void play();
  void pause();
  void stop();
  void changeCurrentMedia(const QUrl& url);
  void endOfMediaList();
#pragma clang diagnostic pop

 public slots:
  void onChangePlayOrder();
  void onNextMedia();
  void onChangeMetaData();
 private slots:

  void onRemoveMedia(const QUrl& url);

 private:
  auto findMedia(const QUrl& url) -> QList<QPointer<MediaItemBox>>::iterator;
  void checkCurrentMedia(const QUrl& url);
  void initDatabase();

 private:
  PlayOrder playOrder_;
  QPointer<Player> player_;
  QPointer<QMediaPlayer> mediaPlayer_;
  QSharedPointer<MediaListSql> database_;

  qsizetype currentIndex_;
  QPointer<MediaItemBox> currentMediaItem_;
  QList<QPointer<MediaItemBox>> mediaList_;
};

#endif  // EUTERPE_MEDIALIST_MEDIALIST_H_
