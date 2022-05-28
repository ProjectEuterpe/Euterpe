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

#ifndef EUTERPE_PLAYERCONTROLLER_PLAYERCONTROLLER_H_
#define EUTERPE_PLAYERCONTROLLER_PLAYERCONTROLLER_H_

#include "../MediaList/MediaItemBox.h"
#include "../MediaList/MediaList.h"
#include "../MetaData/MetaData.h"
#include "../Player/GetFrameData.h"
#include "../Player/Player.h"
#include "PlayerShortcut.h"

class PlayerController : public QWidget {
  Q_OBJECT

 public:
  explicit PlayerController(const QPointer<Player> &player);
  ~PlayerController() override = default;

  //整理获取元数据
  //获取单一元数据
  QVariant getMetaMes(QMediaMetaData::Key key);

  // 初始化媒体库列表
  void initMediaList();
  void addMediaItem(QUrl url);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
 signals:
  void play();
  void pause();
  void stop();
  void prev();
  void next();
  void info();
  void btn_volume();
  void btn_play_order();
  void changeRate(qreal rate);
  void changeProgress(qint64 progress);
  void changeVolume(qreal volume);
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
  void onClickBtnVolume();
  void atEnd();
  void onTimerStart();
  void onTimerEnd();
  //查看url是视频还是音频
  void checkUrl();
  //进度条实现帧图展现
  void onProgressMouseOn(const double);
  void showFrameData(QImage image);
  //快捷键使用部分
  void setVolumeValue(const int add);
  void setProgressValue(const int add);
  void onChangeMetaData();
  void onChangeDuration();
  void onChangeCurrMedia(QUrl url);

 private:
  //音视频控制
  void playVideo();
  void playAudio();
  QPointer<Player> player_;
  //鼠标不移动，计时5秒
  QPointer<QTimer> showBarTimer;
  //计时300ms，关闭展示的Frame
  QPointer<QTimer> showFrameTimer;
  QPointer<GetFrameData> frameData;
  QPointer<MediaItemBox> curMediaItemBox;
  QPointer<MediaList> mediaList;
  QPointer<PlayerShortcut> shortcut_;
};

#endif  // EUTERPE_PLAYERCONTROLLER_PLAYERCONTROLLER_H_
