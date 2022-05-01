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

#include <QPointer>

#include "Player.h"

class PlayerController : public QWidget {
  Q_OBJECT

 public:
  explicit PlayerController(const QPointer<Player> &player);
  ~PlayerController() override = default;

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

 private:
  QPointer<Player> player_;
  enum PlayOrder { onlyOnce = 1, inOrder, randomLoop, singleLoop} playOrder = onlyOnce;
};

#endif  // EUTERPE__PLAYERCONTROLLER_H_
