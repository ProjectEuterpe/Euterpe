/**
 * @file MediaListSql.h
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

#ifndef EUTERPE_MEDIALIST_MEDIALISTSQL_H_
#define EUTERPE_MEDIALIST_MEDIALISTSQL_H_

#include <QSqlTableModel>
#include <QtSql>

struct ConnectionArgs {
  QString driverName;
  QString databaseName;
  QString userName;
  QString password;
  QString hostName;
  qint32 port;
};

struct MediaData {
  QString md5;
  QString mediaName;
  QString mediaPath;
  QString label;
  QString metaData;
  qint64 playTimestamp;
  qint64 addTimestamp;
};

enum class MediaDataEnum {
  MEDIA_PATH = 0,
  MEDIA_NAME = 1,
  MD5 = 2,
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
  mu explicit MediaListSql(const QSharedPointer<ConnectionArgs> &args);
  mu explicit MediaListSql(const QByteArray &json);  // json format
  ~MediaListSql() = default;

  auto connect() -> void;
  nd auto table() const -> QList<QSharedPointer<MediaData>>;
  nd auto sort(const MediaDataEnum &dataEnum, const SortEnum &sortEnum) const
      -> QList<QSharedPointer<MediaData>>;
  nd auto find(const MediaDataEnum &dataEnum, const QVariant &v) const
      -> QList<QSharedPointer<MediaData>>;
  void insert(const QSharedPointer<MediaData> &row) const;
  void remove(const QSharedPointer<MediaData> &row) const;
  void update(const QSharedPointer<MediaData> &row,
              const MediaDataEnum &dataEnum, const QVariant &v) const;

 private:
  nd auto query(const QString &) const -> QList<QSharedPointer<MediaData>>;

  QSqlDatabase db_;
  QSharedPointer<ConnectionArgs> connectionArgs_;
  QMap<MediaDataEnum, QString> mediaDataEnum_ = QMap<MediaDataEnum, QString>({
      {MediaDataEnum::MD5, "md5"},
      {MediaDataEnum::MEDIA_NAME, "mediaName"},
      {MediaDataEnum::MEDIA_PATH, "mediaPath"},
      {MediaDataEnum::LABEL, "label"},
      {MediaDataEnum::METADATA, "metaData"},
      {MediaDataEnum::PLAY_TIMESTAMP, "playTimestamp"},
      {MediaDataEnum::ADD_TIMESTAMP, "addTimestamp"},
  });
  QMap<SortEnum, QString> sortEnum_ = QMap<SortEnum, QString>({
      {SortEnum::DESC, "desc"},
      {SortEnum::ASC, "asc"},
  });
};

#undef mu
#undef nd

#endif  // EUTERPE_MEDIALIST_MEDIALISTSQL_H_
