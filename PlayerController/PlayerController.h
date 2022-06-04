/*
 * @file PlayerController.h
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

#ifndef EUTERPE_PLAYERCONTROLLER_PLAYERCONTROLLER_H_
#define EUTERPE_PLAYERCONTROLLER_PLAYERCONTROLLER_H_

#include <QJsonObject>

#include "../MediaList/MediaItemBox.h"
#include "../MediaList/MediaList.h"
#include "../MetaData/MetaData.h"
#include "../Player/FrameData.h"
#include "../Player/Player.h"
#include "PlayerShortcut.h"

#define nd [[nodiscard]]

class PlayerController : public QWidget {
  Q_OBJECT

 public:
  explicit PlayerController(const QPointer<Player>& player);
  ~PlayerController() override = default;

 private:
  nd auto getMetaData(const QMediaMetaData::Key& key) const -> QVariant;
  void initMediaList();
  void addMediaItem(const QUrl& url) const;
  void playVideo() const;
  void playAudio() const;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
 signals:
  void play();
  void pause();
  void stop();
  void sequence(const Sequence& seq);
  void info();
  void changeProgress(const qint64& progress);
  void changeVolume(const qreal& volume);
  void changeRate(const qreal& rate);
#pragma clang diagnostic pop

 private slots:
  void onClickPlay();
  void onClickStop();
  void onClickPrev();
  void onClickNext();
  void onClickInfo();
  void onChangeProgress();
  void onChangeVolume();
  void onChangeRate();
  void onClickMute();
  void onMediaStatusChanged();
  void onTimerStart();
  void onTimerEnd();
  void onChangeMetaData();
  void onChangeDuration();
  void onProgressMouseOn(const qfloat16& percent);
  void onChangeCurrentMedia(const QUrl& url);
  void checkUrl();
  void showFrameData(const QImage& image);
  void showInitWidget();
  void setVolumeValue(const qint32& delta);
  void setProgressValue(const qint32& delta);

 private:
  QPointer<Player> player_;
  QPointer<QTimer> showBarTimer_;
  QPointer<FrameData> frameData_;
  QPointer<MediaList> mediaList_;
  QPointer<PlayerShortcut> shortcut_;
};

#undef nd

#endif  // EUTERPE_PLAYERCONTROLLER_PLAYERCONTROLLER_H_
