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

//
// Created by Mikra Selene on 2022/5/27.
//

#ifndef EUTERPE_MEDIALIST_MEDIALISTSQL_H_
#define EUTERPE_MEDIALIST_MEDIALISTSQL_H_

#include <QJsonObject>
#include <QSqlTableModel>
#include <QString>
#include <QtSql>
#include <memory>

struct ConnectionArgs {
  QString driver_name;
  QString database_name;
  QString user_name;
  QString password;
  QString host_name;
  qint32 port;
};

struct MediaData {
  QString md5;
  QString media_name;
  QString media_path;
  QString label;
  QString metadata;
  qint32 play_timestamp;
  qint32 add_timestamp;
};

enum class MediaDataEnum {
  MD5 = 0,
  MEDIA_NAME = 1,
  MEDIA_PATH = 2,
  LABEL = 3,
  METADATA = 4,
  PLAY_TIMESTAMP = 5,
  ADD_TIMESTAMP = 6,
};

enum class SortEnum { DESC, ASC };
#define nd [[nodiscard]]
#define mu [[maybe_unused]]

class MediaListSql {
 public:
  mu explicit MediaListSql(const std::shared_ptr<ConnectionArgs> &);
  mu explicit MediaListSql(const QByteArray &);  // json format
  ~MediaListSql() = default;

  auto connect() -> void;
  nd auto table() const -> QList<std::shared_ptr<MediaData>>;
  nd auto sort(MediaDataEnum, SortEnum) const
      -> QList<std::shared_ptr<MediaData>>;
  nd auto find(MediaDataEnum, const QVariant &) const
      -> QList<std::shared_ptr<MediaData>>;
  void insert(const std::shared_ptr<MediaData> &) const;
  void remove(const std::shared_ptr<MediaData> &) const;

 public:
  nd auto query(const QString &) const -> QList<std::shared_ptr<MediaData>>;

  QSqlDatabase db_;
  std::shared_ptr<ConnectionArgs> connectionArgs_;
  QMap<MediaDataEnum, QString> mediaDataEnum_ = QMap<MediaDataEnum, QString>({
      {MediaDataEnum::MD5, "md5"},
      {MediaDataEnum::MEDIA_NAME, "media_name"},
      {MediaDataEnum::MEDIA_PATH, "media_path"},
      {MediaDataEnum::LABEL, "label"},
      {MediaDataEnum::METADATA, "metadata"},
      {MediaDataEnum::PLAY_TIMESTAMP, "play_timestamp"},
      {MediaDataEnum::ADD_TIMESTAMP, "add_timestamp"},
  });
  QMap<SortEnum, QString> sortEnum_ = QMap<SortEnum, QString>({
      {SortEnum::DESC, "desc"},
      {SortEnum::ASC, "asc"},
  });
};

#endif  // EUTERPE_MEDIALIST_MEDIALISTSQL_H_
