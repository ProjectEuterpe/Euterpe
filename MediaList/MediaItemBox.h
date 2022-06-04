/**
 * @file MediaItemBox.h
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

#ifndef EUTERPE_MEDIALIST_MEDIAITEMBOX_H_
#define EUTERPE_MEDIALIST_MEDIAITEMBOX_H_

#include <QGroupBox>
#include <QMediaMetaData>
#include <QMediaPlayer>

#include "../MetaData/MetaData.h"
#include "../Player/Player.h"

namespace Ui {
class MediaItemBox;
}
using MediaItemBoxUiPtr = Ui::MediaItemBox *;

#define nd [[nodiscard]]
#define mu [[maybe_unused]]

class MediaItemBox : public QGroupBox {
  Q_OBJECT

 public:
  explicit MediaItemBox(const QPointer<Player> &parent = Q_NULLPTR);
  ~MediaItemBox() override;

  mu void setMediaTitle(const QString &title);
  mu void setMediaArtist(const QString &artist);
  [[maybe_unused]] mu void setImage(const QImage &img);
  mu void setMetaData(const QMediaMetaData &metaData);
  mu void setMediaUrl(const QUrl &newMedia_url);
  mu void setButtonPlay(bool play);
  mu void setActive(bool active);
  nd auto getMediaUrl() const -> QUrl;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
 signals:
  void play(QUrl url);
  void pause();
  void stop();
  void removeMedia(QUrl url);
#pragma clang diagnostic pop

 private slots:
  void onClickPlay();
  void onClickInfo();
  void onClickDel();

 private:
  MediaItemBoxUiPtr ui_;
  QSharedPointer<MetaData> metaData_;
  QString metaDataString_;
  QPointer<Player> player_;
  QUrl mediaUrl_;
  bool isPlaying_;
};

#undef mu
#undef nd

#endif  // EUTERPE_MEDIALIST_MEDIAITEMBOX_H_
