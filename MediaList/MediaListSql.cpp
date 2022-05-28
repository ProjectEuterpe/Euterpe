/*
 * @file
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

#include "MediaListSql.h"

/**
 * @brief
 * @param args
 */
MediaListSql::MediaListSql(const std::shared_ptr<ConnectionArgs> &args) {
  this->connectionArgs_ = args;
}

/**
 * @brief
 * @param json
 */
#define SET_STRING(key)                              \
  if (object.contains(#key)) {                       \
    QJsonValue value = object.value(#key);           \
    if (value.isString()) {                          \
      this->connectionArgs_->key = value.toString(); \
    }                                                \
  }
#define SET_INT(key)                              \
  if (object.contains(#key)) {                    \
    QJsonValue value = object.value(#key);        \
    if (value.isDouble()) {                       \
      this->connectionArgs_->key = value.toInt(); \
    }                                             \
  }
MediaListSql::MediaListSql(const QByteArray &json) {
  QJsonParseError error;
  QJsonDocument document = QJsonDocument::fromJson(json, &error);
  if (!document.isNull() && error.error == QJsonParseError::NoError) {
    if (document.isObject()) {
      QJsonObject object = document.object();
      SET_STRING(driver_name)
      SET_STRING(database_name)
      SET_STRING(user_name)
      SET_STRING(password)
      SET_STRING(host_name)
      SET_INT(port)
    }
  }
}
#undef SET_STRING
#undef SET_INT

void MediaListSql::connect() {
  auto args = this->connectionArgs_;
  this->db_ = QSqlDatabase::addDatabase(args->driver_name,
                                        QString(args->database_name));
  this->db_.setDatabaseName(args->database_name);
  this->db_.setHostName(args->host_name);
  this->db_.setPort(args->port);

  if (!this->db_.open(args->user_name, args->password)) {
    this->db_ = QSqlDatabase();
    QSqlDatabase::removeDatabase(QString(args->database_name));
  }

  // std::ignore = this->query("drop table Media");
  std::ignore = this->query(
      "create table Media ("
      "media_path varchar(255) primary key,"
      "media_name varchar(255),"
      "md5 varchar(255),"
      "label varchar(255),"
      "metadata varchar(255),"
      "play_timestamp int,"
      "add_timestamp int)");
}

auto MediaListSql::query(const QString &sql) const
    -> QList<std::shared_ptr<MediaData>> {
  QSqlQuery query(sql, this->db_);
  auto mediaList = QList<std::shared_ptr<MediaData>>();
  while (query.next()) {
    auto result = std::make_shared<MediaData>(MediaData{
        query.value(qint32(MediaDataEnum::MD5)).toString(),
        query.value(qint32(MediaDataEnum::MEDIA_NAME)).toString(),
        query.value(qint32(MediaDataEnum::MEDIA_PATH)).toString(),
        query.value(qint32(MediaDataEnum::LABEL)).toString(),
        query.value(qint32(MediaDataEnum::METADATA)).toString(),
        query.value(qint32(MediaDataEnum::PLAY_TIMESTAMP)).toInt(),
        query.value(qint32(MediaDataEnum::ADD_TIMESTAMP)).toInt(),
    });
    mediaList.push_back(result);
  }
  return mediaList;
}

auto MediaListSql::table() const -> QList<std::shared_ptr<MediaData>> {
  return this->query("select * from Media");
}

auto MediaListSql::sort(MediaDataEnum key, SortEnum how) const
    -> QList<std::shared_ptr<MediaData>> {
  auto sortString =
      QString("select * from Media order by %1 %2")
          .arg(this->mediaDataEnum_.value(key), this->sortEnum_.value(how));
  return this->query(sortString);
}

auto MediaListSql::find(MediaDataEnum key, const QVariant &value) const
    -> QList<std::shared_ptr<MediaData>> {
  bool keyIsInt = key == MediaDataEnum::PLAY_TIMESTAMP ||
                  key == MediaDataEnum::ADD_TIMESTAMP;
  auto findString =
      QString("select * from Media where %1=%2")
          .arg(this->mediaDataEnum_.value(key),
               QString(keyIsInt ? "%1" : "'%1'").arg(value.toString()));
  return this->query(findString);
}

void MediaListSql::insert(const std::shared_ptr<MediaData> &row) const {
  auto insertString =
      QString("insert into Media values ('%1\','%2\','%3','%4','%5',%6,%7)")
          .arg(row->media_path, row->media_name, row->md5, row->label,
               row->metadata, QString::number(row->play_timestamp),
               QString::number(row->add_timestamp));
  std::ignore = this->query(insertString);
}

void MediaListSql::remove(const std::shared_ptr<MediaData> &row) const {
  auto removeString =
      QString("delete from Media where media_name='%1'").arg(row->media_path);
  std::ignore = this->query(removeString);
}
