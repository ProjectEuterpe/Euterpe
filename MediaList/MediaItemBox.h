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

#ifndef EUTERPE_MEDIALIST_MEDIAITEMBOX_H_
#define EUTERPE_MEDIALIST_MEDIAITEMBOX_H_

#include <QGroupBox>
#include <QMediaMetaData>
#include <QMediaPlayer>

#include "../Player/Player.h"

namespace Ui {
class MediaItemBox;
}

class MediaItemBox : public QGroupBox {
  Q_OBJECT

 public:
  explicit MediaItemBox(Player *parent = nullptr);
  ~MediaItemBox();
  void setTitle(QString title);
  void setArtist(QString author);
  void setImage(QImage img);
  void setMetaData(QMediaMetaData data);
  void setMediaUrl(const QUrl &newMedia_url);
  QUrl getMediaUrl();
  void setBtnPlay(bool play);
  void setActive(bool active);

 private:
  Ui::MediaItemBox *ui_;
  QMediaMetaData metadata_;
  QString metadata_str_;
  Player *player_;
  QUrl media_url_;
  bool isPlaying;

 signals:
  void play(QUrl url);
  void pause();
  void stop();
  void deleteMedia(QUrl url);

 private slots:
  void onClickPlay();
  void onClickBtnInfo();
  void onClickBtnDel();
};

#endif  // EUTERPE_MEDIALIST_MEDIAITEMBOX_H_
