/**
 * @file player.h
 * @author Mikra Selene
 * @version 1.0
 * @date 2022.04.05
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

#ifndef EUTERPE__PLAYER_H_
#define EUTERPE__PLAYER_H_

#include <QAudioOutput>
#include <QMediaPlayer>
#include <QPointer>
#include <QVariant>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Player;
}
using PlayerUiPtr = Ui::Player *;
QT_END_NAMESPACE

class Player : public QWidget {
  Q_OBJECT

 public:
  explicit Player(const QPointer<QWidget> &parent = nullptr);
  ~Player() override;

 public:
  void setLoopLock(bool lock);
  void setProgressSliderMax(int max);
  void setButtonLabelPlay(bool play);
  void setButtonVolume(bool volume);
  void setPlayOrderIcon(int type);
  void addFloatTable(float x, float y, QString text);

#define nd [[nodiscard]]
  nd auto ui() const -> PlayerUiPtr;
  nd auto mediaPlayer() const -> QPointer<QMediaPlayer>;
  nd auto audioOutput() const -> QPointer<QAudioOutput>;
  nd bool rewind() const;
  nd bool loop() const;
  nd auto duration() const -> qint64;
  nd auto totalTime() const -> qint64;
  nd auto metaData() const -> QMediaMetaData;
  nd auto state() const -> QMediaPlayer::PlaybackState;
  nd auto sliderVolume() const -> float;
  nd auto sliderProgress() const -> qint64;
  nd auto comboBoxRate() const -> qreal;
  nd bool endOfMedia() const;
#undef nd

 private slots:
  void progressing(qint64 progress);
  void onClickOpen();

 private:
  void initMedia(const QUrl &url);
  void updateTimeLabel(qint64 time);

 private:
  PlayerUiPtr ui_;
  QPointer<QMediaPlayer> media_player_;
  QPointer<QAudioOutput> audio_output_;
};

#endif  // EUTERPE__PLAYER_H_
