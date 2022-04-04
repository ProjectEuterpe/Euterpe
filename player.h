/*
 * Copyright (C) 2022 Mikra Selene
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EUTERPE__PLAYER_H_
#define EUTERPE__PLAYER_H_

#include <QAudioOutput>
#include <QFileDialog>
#include <QMediaMetaData>
#include <QMediaPlayer>

#define nd [[nodiscard]]

QT_BEGIN_NAMESPACE
namespace Ui {
class Player;
}
QT_END_NAMESPACE

class Player : public QWidget {
  Q_OBJECT

 public:
  explicit Player(QWidget *parent = nullptr);
  ~Player() override;

 public:
  nd auto Duration() const -> qint64;
  nd auto TotalTime() const -> qint64;
  nd auto MetaData() const -> QMediaMetaData;
  nd auto State() const -> QMediaPlayer::PlaybackState;
  nd auto SliderVolume() const -> float;
  nd auto ComboBoxRate() const -> qreal;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
 signals:
  void Play();
  void Pause();
  void Stop();
  void Prev();
  void Next();
  void Info();
  void ChangeRate(qreal rate);
  void ChangeProgress(qint64 progress);
  void ChangeVolume(float volume);
#pragma clang diagnostic pop

 private slots:
  void OnClickOpen();
  void OnClickPlay();
  void OnClickStop();
  void OnClickPrev();
  void OnClickNext();
  void OnClickInfo();
  void OnChangeProgress();
  void OnChangeVolume();
  void OnChangeRate();
  void AtEnd();
  void Progressing(qint64);

 private:
  void InitMedia();
  void UpdateTimeLabel(qint64);

  QMediaPlayer *player_;
  QAudioOutput *audio_;
  Ui::Player *ui_;
};

#undef nd

#endif  // EUTERPE__PLAYER_H_
