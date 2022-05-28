/**
 * @file PlayerShortcut.h
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

#ifndef EUTERPE_PLAYERCONTROLLER_PLAYERSHORTCUT_H_
#define EUTERPE_PLAYERCONTROLLER_PLAYERSHORTCUT_H_

#include <QShortcut>

#include "../Player/Player.h"

class PlayerShortcut : public QObject {
  Q_OBJECT
  enum class SliderShortcut { Volume, Progress };

 public:
  explicit PlayerShortcut(const QPointer<Player>& parent = Q_NULLPTR);
  ~PlayerShortcut() override = default;

  auto generateShortcut(const QString& shortcut) -> QPointer<QShortcut>;
  void addShortcut(const QString& shortcut, QPushButton* button);
  void addShortcut(const QString& shortcut, const qint32& delta,
                   const SliderShortcut& choose);
  void removeShortcut(const QString& shortcut);

  void initBasicShortcuts();
  void playAudio();
  void playVideo();

 signals:
  void changeVolume(int add);
  void changeProgress(int add);

 private:
  QPointer<Player> player_;
  QMap<QString, QPointer<QShortcut>> shortcutList_;
};

#endif  // EUTERPE_PLAYERCONTROLLER_PLAYERSHORTCUT_H_
