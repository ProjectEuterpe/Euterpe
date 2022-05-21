/**
 * @file main.cpp
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

#include <QApplication>
#include <QFile>

#include "Player.h"
#include "FloatTable.h"
#include"PlayerController.h"

int main(int argc, char* argv[]) {
  auto app = QPointer<QApplication>(new QApplication(argc, argv));
  auto player = QPointer<Player>(new Player(nullptr));
  auto controller = QPointer<PlayerController>(new PlayerController(player));

  /* read qss*/
  QString qss;
  QFile qssFile(":/qss/main.qss");
  qssFile.open(QFile::ReadOnly);
  if(qssFile.isOpen()){
    qss = QLatin1String(qssFile.readAll());
    qApp->setStyleSheet(qss);
    qssFile.close();
  }

  player->show();
  QApplication::exec();

}
