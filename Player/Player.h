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

#ifndef EUTERPE__PLAYER_H_
#define EUTERPE__PLAYER_H_

#include <QAudioOutput>
#include <QFileDialog>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QPointer>
#include <QTime>
#include <QVariant>
#include <QWidget>
#include <QStackedWidget>

#include "../MetaData/MetaDataFloatTable.h"

namespace Ui {
class Player;
}
using PlayerUiPtr = Ui::Player *;

enum class PlayOrder { OnlyOnce, InOrder, Random, SingleLoop };

class Player : public QWidget {
  Q_OBJECT

 public:
  explicit Player(const QPointer<QWidget> &parent = Q_NULLPTR);
  ~Player() override;

 public:
  void setLoopLock(bool lock);
  void setProgressSliderMax(const qint32 &max);
  void setButtonPlayIcon(bool play);
  void setButtonVolumeIcon(bool unmute);
  void setPlayOrderIcon(const PlayOrder &type);

  void setIsFullScreenIcon();
  void changeFullScreenStatus();

  // TODO:
  void addFloatTable(float x, float y, QString text);

  //展示帧图相关
  void setFrame(QImage image);
  void setFramePos(float x);

  void setMediaUrl(const QUrl &newMedia_url);
  void addFloatTable(QPushButton *info, QString text, int posType = 0);
  void addMediaItemBox(QWidget *widget);
  void deleteMediaItemBox(QWidget *widget);
  void addMediaItemSpacerV();

#define nd [[nodiscard]]
  nd auto ui() const -> PlayerUiPtr;
  nd auto mediaPlayer() const -> QPointer<QMediaPlayer>;
  nd auto audioOutput() const -> QPointer<QAudioOutput>;
  nd bool rewind() const;
  nd auto duration() const -> qint64;
  nd auto totalTime() const -> qint64;
  nd auto metaData() const -> QMediaMetaData;
  nd auto state() const -> QMediaPlayer::PlaybackState;
  nd auto sliderVolume() const -> qreal;
  nd auto sliderProgress() const -> qint64;
  nd auto comboBoxRate() const -> qreal;
  nd bool endOfMedia() const;
  nd auto url()const ->QUrl;

 public slots:
  //定时关闭展示的frame
  void closeFrameShow();
  void playMedia();
  void stopMedia();
  void pauseMedia();
#undef nd

 signals:
  //全屏状态下，展示工具栏
  void showBar();
  // 导入媒体
  void addMedia(QUrl url);

 private slots:
  void progressing(qint64 progress);
  void onClickOpen();
  void onClickFullScreen();
  bool eventFilter(QObject *, QEvent *) override;

 private:
  void initMedia(const QUrl &url);
  void updateTimeLabel(qint64 time);

 private:
  PlayerUiPtr ui_;
  QPointer<QMediaPlayer> mediaPlayer_;
  QPointer<QAudioOutput> audioOutput_;
  bool isFullScreen_;
  QPointer<MetaDataFloatTable> frame_;
  QPointer<QStackedWidget> stacked_widget;

 public:
  QUrl mediaUrl_;
};

#endif  // EUTERPE__PLAYER_H_
