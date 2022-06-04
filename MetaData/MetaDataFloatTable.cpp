/**
 * @file MetaDataFloatTable.cpp
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

#include "MetaDataFloatTable.h"

MetaDataFloatTable::MetaDataFloatTable(QWidget *parent)
    : QWidget(parent), ui_(new Ui::MetaDataFloatTable) {
  this->ui_->setupUi(this);
  QFont font;
  font.setPixelSize(12);
  this->ui_->textEdit->setFont(font);
}

MetaDataFloatTable::~MetaDataFloatTable() { delete ui_; }

void MetaDataFloatTable::setPosition(const qint32 &x, const qint32 &y) {
  this->setGeometry(x, y, this->ui_->textEdit->width(),
                    this->ui_->textEdit->height());
}

void MetaDataFloatTable::setText(const QString &text) {
  this->ui_->textEdit->setText(
      text.isEmpty() ? tr("You haven't selected a media to play ~") : text);
}

void MetaDataFloatTable::setImage(const QImage &image) {
  this->ui_->textEdit->setText(tr(""));
  this->ui_->textEdit->textCursor().insertImage(image);
}
