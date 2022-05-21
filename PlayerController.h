/**
 * @file playercontroller.h
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

#ifndef EUTERPE__PLAYERCONTROLLER_H_
#define EUTERPE__PLAYERCONTROLLER_H_
#include <QMediaMetaData>
#include <QPointer>
#include <QTimer>

#include "Player.h"
#include "MediaItemBox.h"
#include "GetFrameData.h"
#include"Player_Shortcut.h"
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
  void addMediaItem(QMediaMetaData metaData);

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
  void changeVolume(float volume);

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
  void onClickBtnPlayOrder();
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

 private:
  //音视频控制
  void playVideo();
  void playAudio();
  QPointer<Player> player_;
  enum PlayOrder { onlyOnce = 1, inOrder, randomLoop, singleLoop} playOrder = onlyOnce;
  //鼠标不移动，计时5秒
QPointer<QTimer>showBarTimer;
//计时300ms，关闭展示的Frame
QPointer<QTimer>showFrameTimer;
QPointer<GetFrameData>frameData;
  QPointer<MediaItemBox> curMediaItemBox;
  QList<QPointer<MediaItemBox>> mediaListBox;
QPointer<Player_Shortcut>shortcut;
};

#endif  // EUTERPE__PLAYERCONTROLLER_H_
