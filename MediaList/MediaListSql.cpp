/**
 * @file MediaListSql.cpp
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

#include "MediaListSql.h"

/**
 * @brief MediaListSql.
 * @param args: Connection args.
 */
MediaListSql::MediaListSql(const QSharedPointer<ConnectionArgs> &args)
    : connectionArgs_(args) {}

/**
 * @brief MediaListSql.
 * @param json: Connection args in json format.
 */
MediaListSql::MediaListSql(const QByteArray &json) {
  QJsonParseError error;
  auto document = QJsonDocument::fromJson(json, &error);
  if (!document.isNull() && error.error == QJsonParseError::NoError) {
    if (document.isObject()) {
      auto object = document.object();
#define SET_STRING(key)                              \
  if (object.contains(#key)) {                       \
    auto value = object.value(#key);                 \
    if (value.isString()) {                          \
      this->connectionArgs_->key = value.toString(); \
    }                                                \
  }
#define SET_INT(key)                              \
  if (object.contains(#key)) {                    \
    auto value = object.value(#key);              \
    if (value.isDouble()) {                       \
      this->connectionArgs_->key = value.toInt(); \
    }                                             \
  }
      SET_STRING(driverName)
      SET_STRING(databaseName)
      SET_STRING(userName)
      SET_STRING(password)
      SET_STRING(hostName)
      SET_INT(port)
#undef SET_STRING
#undef SET_INT
    }
  }
}

/**
 * @brief Connect to a database using connection args.
 */
void MediaListSql::connect() {
  auto args = this->connectionArgs_;
  this->db_ =
      QSqlDatabase::addDatabase(args->driverName, QString(args->databaseName));
  this->db_.setDatabaseName(args->databaseName);
  this->db_.setHostName(args->hostName);
  this->db_.setPort(args->port);

  if (!this->db_.open(args->userName, args->password)) {
    this->db_ = QSqlDatabase();
    QSqlDatabase::removeDatabase(QString(args->databaseName));
  }

  // std::ignore = this->query("drop table Media");
  std::ignore = this->query(
      "create table Media ("
      "media_path varchar(255) primary key,"
      "media_name varchar(255),"
      "md5 varchar(255),"
      "label varchar(255),"
      "meta_data varchar(2048),"
      "play_timestamp bigint,"
      "add_timestamp bigint)");
}

/**
 * @brief Get the table.
 * @return Whole table (media database).
 */
auto MediaListSql::table() const -> QList<QSharedPointer<MediaData>> {
  return this->query("select * from Media");
}

/**
 * @brief Sort the table.
 * @param key: Sort by which key.
 * @param how: Sort in ascending order or in descending order.
 * @return Result.
 */
[[maybe_unused]] auto MediaListSql::sort(const MediaDataEnum &key,
                                         const SortEnum &how) const
    -> QList<QSharedPointer<MediaData>> {
  auto sortString =
      QString("select * from Media order by %1 %2")
          .arg(this->mediaDataEnum_.value(key), this->sortEnum_.value(how));
  return this->query(sortString);
}

/**
 * @brief Find rows.
 * @param key: Find in which key.
 * @param value: Find by what value.
 * @return Result.
 */
auto MediaListSql::find(const MediaDataEnum &key, const QVariant &value) const
    -> QList<QSharedPointer<MediaData>> {
  bool keyIsInt = key == MediaDataEnum::PLAY_TIMESTAMP ||
                  key == MediaDataEnum::ADD_TIMESTAMP;
  auto findString =
      QString("select * from Media where %1=%2")
          .arg(this->mediaDataEnum_.value(key),
               QString(keyIsInt ? "%1" : "'%1'").arg(value.toString()));
  return this->query(findString);
}

/**
 * @brief Insert a row.
 * @param row: The row to be inserted.
 */
void MediaListSql::insert(const QSharedPointer<MediaData> &row) const {
  QSqlQuery query(this->db_);
  query.prepare(
      "insert into Media values"
      "(:media_path, :media_name, :md5, :label, :meta_data, :play_timestamp, "
      ":add_timestamp)");
  query.bindValue(":media_path", row->mediaPath);
  query.bindValue(":media_name", row->mediaName);
  query.bindValue(":md5", row->md5);
  query.bindValue(":label", row->label);
  query.bindValue(":meta_data", row->metaData);
  query.bindValue(":play_timestamp", row->playTimestamp);
  query.bindValue(":add_timestamp", row->addTimestamp);
  qDebug() << query.exec();
}

/**
 * @brief Remove a row.
 * @param row: The row to be removed.
 */
void MediaListSql::remove(const QSharedPointer<MediaData> &row) const {
  auto removeString =
      QString("delete from Media where media_path='%1'").arg(row->mediaPath);
  qDebug() << removeString;
  std::ignore = this->query(removeString);
}

/**
 * @brief Update a key's value of a row to given value.
 * @param row: The row to be updated.
 * @param key: The key to be updated.
 * @param value: The value.
 */
void MediaListSql::update(const QSharedPointer<MediaData> &row,
                          const MediaDataEnum &key,
                          const QVariant &value) const {
  bool keyIsInt = key == MediaDataEnum::PLAY_TIMESTAMP ||
                  key == MediaDataEnum::ADD_TIMESTAMP;
  auto updateString =
      QString("update Media set %1=%2 where media_path='%3'")
          .arg(this->mediaDataEnum_.value(key),
               QString(keyIsInt ? "%1" : "'%1'").arg(value.toString()))
          .arg(row->mediaPath);
  std::ignore = this->query(updateString);
}

/**
 * @brief A SQL query.
 * @param sql: SQL query.
 * @return Media list (if possible).
 */
auto MediaListSql::query(const QString &sql) const
    -> QList<QSharedPointer<MediaData>> {
  QSqlQuery query(sql, this->db_);
  auto mediaList = QList<QSharedPointer<MediaData>>();
  while (query.next()) {
    auto result = QSharedPointer<MediaData>(new MediaData{
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
