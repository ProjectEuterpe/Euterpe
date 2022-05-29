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

#ifndef EUTERPE_MEDIALIST_MEDIALIST_H_
#define EUTERPE_MEDIALIST_MEDIALIST_H_

#include "../Player/GetFrameData.h"
#include "../Player/Player.h"
#include "MediaItemBox.h"
#include "MediaListSql.h"

class MediaList : public QObject {
  Q_OBJECT
 private:
  PlayOrder playOrder_;
  QPointer<Player> player_;
  QPointer<QMediaPlayer> mediaPlayer_;
  QList<QPointer<MediaItemBox>> mediaList_;
  QPointer<MediaItemBox> currMediaItem_;
  QPointer<MediaListSql> database_;

  int currIndex;

  void checkCurrMedia(QUrl url);

 public:
  explicit MediaList(QPointer<Player> player_);
  int findMediaItem(QUrl url);
  void addMediaItemBox(QPointer<MediaItemBox> mediaItemBox);
  void importMedia(const QUrl& url);
  void playCurrMedia();
  void stopCurrMedia();

 private:
  void initDatabase();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
 signals:
  void play();
  void pause();
  void stop();
  void changeCurrMedia(QUrl url);
    void endMedialist();
#pragma clang diagnostic pop

 private slots:
  void onChangeMetaData();
  void deleteMedia(QUrl url);

 public slots:
  void onChangePlayOrder();
  void onNextMedia();
};

#endif  // EUTERPE_MEDIALIST_MEDIALIST_H_
