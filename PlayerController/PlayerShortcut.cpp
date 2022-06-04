/**
 * @file PlayerShortcut.cpp
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

#include "PlayerShortcut.h"

#include "../UI/ui_Player.h"

/**
 * @brief Player shortcut.
 * @param parent
 */
PlayerShortcut::PlayerShortcut(const QPointer<Player> &player)
    : player_(player) {
  this->shortcutList_.clear();
  this->initBasicShortcuts();
}

/**
 * @brief Generate shortcut.
 * @param shortcut
 * @return
 */
auto PlayerShortcut::generateShortcut(const QString &shortcut)
    -> QPointer<QShortcut> {
  auto newShortcut = QPointer{new QShortcut(QKeySequence(shortcut), player_)};
  newShortcut->setContext(Qt::ApplicationShortcut);
  this->shortcutList_.insert(shortcut, newShortcut);
  return newShortcut;
}

/**
 * @brief Add (connect) a shortcut to a button. if the shortcut has already been
 * connected, do nothing.
 * @param shortcut
 * @param button
 */
void PlayerShortcut::addShortcut(const QString &shortcut, QPushButton *button) {
  if (!this->shortcutList_.contains(shortcut)) {
    connect(this->generateShortcut(shortcut), &QShortcut::activated, button,
            &QPushButton::click);
  }
}

/**
 * @brief Add (connect) a shortcut to a slider. if the shortcut has already been
 * connected, do nothing.
 * @param shortcut
 * @param delta: Amount of change.
 * @param choose
 */
void PlayerShortcut::addShortcut(const QString &shortcut, const qint32 &delta,
                                 const SliderShortcut &choose) {
  if (!this->shortcutList_.contains(shortcut)) {
    connect(this->generateShortcut(shortcut), &QShortcut::activated, this,
            [=]() {
              emit choose == SliderShortcut::Volume ? changeVolume(delta)
                                                    : changeProgress(delta);
            });
  }
}

/**
 * @brief Remove the connection of a shortcut.
 * @param shortcut
 */
void PlayerShortcut::removeShortcut(const QString &shortcut) {
  auto target = this->shortcutList_.find(shortcut);
  if (target != this->shortcutList_.end()) {
    disconnect(target.value(), Q_NULLPTR, Q_NULLPTR, Q_NULLPTR);
    delete target.value();
    this->shortcutList_.remove(shortcut);
  }
}

/**
 * @brief ...
 */
void PlayerShortcut::initBasicShortcuts() {
  this->addShortcut("space", this->player_->ui()->play);
  this->addShortcut("ctrl+left", this->player_->ui()->prev);
  this->addShortcut("ctrl+right", this->player_->ui()->next);
  this->addShortcut("ctrl+i", this->player_->ui()->open);
  this->addShortcut("ctrl+f", this->player_->ui()->isfullScreen);
  this->addShortcut("ctrl+down", -20, SliderShortcut::Volume);
  this->addShortcut("ctrl+up", +20, SliderShortcut::Volume);
  this->addShortcut("down", +5, SliderShortcut::Progress);
  this->addShortcut("up", -5, SliderShortcut::Progress);
  this->addShortcut("left", -1, SliderShortcut::Progress);
  this->addShortcut("right", +1, SliderShortcut::Progress);
}

/**
 * @brief ...
 */
void PlayerShortcut::playAudio() {
  this->removeShortcut("down");
  this->removeShortcut("up");
  this->removeShortcut("left");
  this->removeShortcut("right");
}

/**
 * @brief ...
 */
void PlayerShortcut::playVideo() {
  this->addShortcut("down", +5, SliderShortcut::Progress);
  this->addShortcut("up", -5, SliderShortcut::Progress);
  this->addShortcut("left", -1, SliderShortcut::Progress);
  this->addShortcut("right", +1, SliderShortcut::Progress);
}
