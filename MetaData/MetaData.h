/**
 * @file MetaData.h
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

#ifndef EUTERPE_METADATA_METADATA_H_
#define EUTERPE_METADATA_METADATA_H_

#include <QJsonDocument>
#include <QJsonObject>
#include <QMediaMetaData>

#define nd [[nodiscard]]
#define mu [[maybe_unused]]

class MetaData {
 public:
  explicit MetaData(QMediaMetaData metaData);
  explicit MetaData(const QString& json);
  ~MetaData() = default;

  mu nd auto get(const QString& key) -> QVariant;
  mu nd auto toQtJsonObject() const -> QJsonObject;
  mu nd auto toQtMetaData() const -> QMediaMetaData;
  mu nd auto toJsonStringCompact() const -> QString;
  mu nd auto toJsonStringIndented() const -> QString;
  mu nd auto toPrettyString() const -> QString;

 private:
  QJsonDocument json_;
  QMediaMetaData metaData_;
};

#endif  // EUTERPE_METADATA_METADATA_H_
