/**
 * @file MetaDataFloatTable.h
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

#ifndef EUTERPE_METADATA_METADATAFLOATTABLE_H_
#define EUTERPE_METADATA_METADATAFLOATTABLE_H_

#include <QWidget>

#include "../UI/ui_MetaDataFloatTable.h"

namespace Ui {
class MetaDataFloatTable;
}
using MetaDataFloatTableUIPtr = Ui::MetaDataFloatTable *;

class MetaDataFloatTable : public QWidget {
  Q_OBJECT

 public:
  explicit MetaDataFloatTable(QWidget *parent = Q_NULLPTR);
  ~MetaDataFloatTable() override;

  void setPosition(const qint32 &x, const qint32 &y);
  void setImage(const QImage &image);
  void setText(const QString &text);

 private:
  MetaDataFloatTableUIPtr ui_;
};

#endif  // EUTERPE_METADATA_METADATAFLOATTABLE_H_
