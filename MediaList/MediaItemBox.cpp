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

#include "MediaItemBox.h"

#include "../UI/ui_MediaItemBox.h"

MediaItemBox::MediaItemBox(Player *parent)
    : QGroupBox(parent), ui_(new Ui::MediaItemBox) {
  ui_->setupUi(this);
  player_ = parent;
  isPlaying = false;

  connect(ui_->btnPlay, &QPushButton::clicked, this,
          &MediaItemBox::onClickPlay);
  connect(ui_->btnInfo, &QPushButton::clicked, this,
          &MediaItemBox::onClickBtnInfo);
  connect(ui_->btnDel, &QPushButton::clicked, this,
          &MediaItemBox::onClickBtnDel);
}

MediaItemBox::~MediaItemBox() { delete ui_; }

void MediaItemBox::setTitle(QString title) {
  if (title == "") title = "Unknown Title";
  ui_->textTitle->setText(title);
  ui_->textTitle->setToolTip(title);
}

void MediaItemBox::setArtist(QString artist) {
  if (artist == "") artist = "Unknown Artist";
  ui_->textArtist->setText(artist);
  ui_->textArtist->setToolTip(artist);
}

void MediaItemBox::setMetaData(QMediaMetaData data) {
  metadata_ = data;
  metadata_str_ = "";
  for (auto k : metadata_.keys()) {
    metadata_str_ +=
        metadata_.metaDataKeyToString(k) + ":" + metadata_[k].toString() + '\n';
  }
}

void MediaItemBox::setMediaUrl(const QUrl &newMedia_url) {
  media_url_ = newMedia_url;
}
QUrl MediaItemBox::getMediaUrl() const { return media_url_; }

void MediaItemBox::setButtonPlay(bool play) {
  qDebug() << "setButtonPlay" << play;
  ui_->btnPlay->setIcon(
      QIcon(play ? ":/images/circle-play.svg" : ":/images/circle-pause.svg"));
}

void MediaItemBox::setActive(bool active) {
  QString color = active ? "background-color: #E5E5E5" : "";
  this->setStyleSheet(color);
  if (!active) {
    isPlaying = false;
    setButtonPlay(!isPlaying);
  } else {
    emit pause();
  }
}

void MediaItemBox::onClickPlay() {
  qDebug() << "onClickPlay" << isPlaying;
  setButtonPlay(isPlaying);
  if (isPlaying) {
    emit pause();
  } else {
    emit play(media_url_);
  }
  isPlaying = !isPlaying;
}

void MediaItemBox::onClickBtnInfo() {
  qDebug() << "clicked: BtnInfo";
  player_->addFloatTable(ui_->btnInfo, metadata_str_, 1);
}

void MediaItemBox::onClickBtnDel() {
  qDebug() << "clicked: BtnDel";
  emit deleteMedia(media_url_);
}
