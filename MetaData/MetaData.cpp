/**
 * @file MetaData.cpp
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

#include "MetaData.h"

/**
 * @brief Meta data.
 * @param metaData
 */
MetaData::MetaData(QMediaMetaData metaData) {
  this->metaData_ = metaData;
  this->metaData_.remove(QMediaMetaData::Comment);  // netease music sucks.
  auto jsonObj = QJsonObject();
  for (auto k : this->metaData_.keys()) {
    jsonObj.insert(QMediaMetaData::metaDataKeyToString(k),
                   QJsonValue::fromVariant(metaData[k]));
  }
  this->json_ = QJsonDocument(jsonObj);
}

MetaData::MetaData(const QString& json) {
  this->metaData_ = {};
  this->json_ = QJsonDocument::fromJson(json.toUtf8());
}

auto MetaData::get(const QString& key) -> QVariant { return this->json_[key]; }

/**
 * @brief To Qt JSON object.
 * @return
 */
auto MetaData::toQtJsonObject() const -> QJsonObject {
  return this->json_.object();
}

/**
 * @brief To Qt media meta data.
 * @return
 */
auto MetaData::toQtMetaData() const -> QMediaMetaData {
  return this->metaData_;
}

/**
 * @brief To compact JSON string.
 * @return A string shaped like "{"key1":1,"key2":"value2"}".
 */
auto MetaData::toJsonStringCompact() const -> QString {
  return this->json_.toJson(QJsonDocument::Compact);
}

/**
 * @brief To indented JSON string.
 * @return A string shaped like "
 *  {
 *      "key1": 1,
 *      "key2": "value2"
 *  }
 * ".
 */
auto MetaData::toJsonStringIndented() const -> QString {
  return this->json_.toJson(QJsonDocument::Indented);
}

/**
 * @brief To pretty formatted string.
 * @return A string shaped like "
 *  key1: 1
 *  key2: value2
 * ".
 */
auto MetaData::toPrettyString() const -> QString {
  auto result = QString();
  for (auto k : this->metaData_.keys()) {
    auto key = QMediaMetaData::metaDataKeyToString(k);
    result.append(key + ": " + this->json_[key].toString() + "\n");
  }
  return result;
}
