/**
 * @file PlayerShortcut.cpp
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

#include "PlayerShortcut.h"

#include "../UI/ui_Player.h"

/**
 * @brief Player shortcut.
 * @param parent
 */
PlayerShortcut::PlayerShortcut(const QPointer<Player> &parent) {
  player_ = parent;
  shortcutList_.clear();
  initBasicShortcuts();
}

/**
 * @brief
 * @param shortcut
 * @return
 */
auto PlayerShortcut::generateShortcut(const QString &shortcut)
    -> QPointer<QShortcut> {
  auto newShortcut = QPointer{new QShortcut(QKeySequence(shortcut), player_)};
  newShortcut->setContext(Qt::ApplicationShortcut);
  shortcutList_.insert(shortcut, newShortcut);
  return newShortcut;
}

/**
 * @brief Add (connect) a shortcut to a button. if the shortcut has already been
 * connected, do nothing.
 * @param shortcut
 * @param button
 */
void PlayerShortcut::addShortcut(const QString &shortcut, QPushButton *button) {
  if (!shortcutList_.contains(shortcut)) {
    connect(generateShortcut(shortcut), &QShortcut::activated, button,
            &QPushButton::click);
  }
}

/**
 * @brief Add (connect) a shortcut to a slider. if the shortcut has already been
 * connected, do nothing.
 * @param shortcut
 * @param delta amount of change
 * @param choose
 */
void PlayerShortcut::addShortcut(const QString &shortcut, const qint32 &delta,
                                 const SliderShortcut &choose) {
  if (!shortcutList_.contains(shortcut)) {
    connect(generateShortcut(shortcut), &QShortcut::activated, this, [=]() {
      if (choose == SliderShortcut::Volume) {
        emit changeVolume(delta);
      } else if (choose == SliderShortcut::Progress) {
        emit changeProgress(delta);
      }
    });
  }
}

/**
 * @brief Remove the connection of a shortcut.
 * @param shortcut
 */
void PlayerShortcut::removeShortcut(const QString &shortcut) {
  auto target = shortcutList_.find(shortcut);
  if (target != shortcutList_.end()) {
    disconnect(target.value(), Q_NULLPTR, Q_NULLPTR, Q_NULLPTR);
    delete target.value();
    shortcutList_.remove(shortcut);
  }
}

/**
 * @brief ...
 */
void PlayerShortcut::initBasicShortcuts() {
  addShortcut("space", player_->ui()->play);
  addShortcut("ctrl+left", player_->ui()->prev);
  addShortcut("ctrl+right", player_->ui()->next);
  addShortcut("ctrl+i", player_->ui()->open);
  addShortcut("ctrl+f", player_->ui()->isfullScreen);
  addShortcut("ctrl+down", -20, SliderShortcut::Volume);
  addShortcut("ctrl+up", +20, SliderShortcut::Volume);
  addShortcut("down", +5, SliderShortcut::Progress);
  addShortcut("up", -5, SliderShortcut::Progress);
  addShortcut("left", -1, SliderShortcut::Progress);
  addShortcut("right", +1, SliderShortcut::Progress);
}

/**
 * @brief ...
 */
void PlayerShortcut::playAudio() {
  removeShortcut("down");
  removeShortcut("up");
  removeShortcut("left");
  removeShortcut("right");
}

/**
 * @brief ...
 */
void PlayerShortcut::playVideo() {
  addShortcut("down", +5, SliderShortcut::Volume);
  addShortcut("up", -5, SliderShortcut::Volume);
  addShortcut("left", -50, SliderShortcut::Progress);
  addShortcut("right", +50, SliderShortcut::Progress);
}
