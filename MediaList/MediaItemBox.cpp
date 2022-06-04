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
  isPlaying_ = false;

  connect(ui_->btnPlay, &QPushButton::clicked, this,
          &MediaItemBox::onClickPlay);
  connect(ui_->btnInfo, &QPushButton::clicked, this,
          &MediaItemBox::onClickBtnInfo);
  connect(ui_->btnDel, &QPushButton::clicked, this,
          &MediaItemBox::onClickBtnDel);
}

MediaItemBox::~MediaItemBox() { delete ui_; }

[[maybe_unused]] void MediaItemBox::setMediaTitle(const QString &title) {
  auto text = title.isEmpty() ? tr("Unknown Title") : title;
  ui_->textTitle->setText(text);
  ui_->textTitle->setToolTip(text);
}

void MediaItemBox::setMediaArtist(const QString &artist) {
  auto text = artist.isEmpty() ? "V/A" : artist;
  ui_->textArtist->setText(text);
  ui_->textArtist->setToolTip(text);
}

void MediaItemBox::setImage(const QImage &img) {}

void MediaItemBox::setMetaData(const QMediaMetaData &data) {
  metaData_ = data;
  metaDataString_ = "";
  for (auto k : metaData_.keys()) {
    metaDataString_ +=
        metaData_.metaDataKeyToString(k) + ":" + metaData_[k].toString() + '\n';
  }
}

void MediaItemBox::setMediaUrl(const QUrl &newMedia_url) {
  mediaUrl_ = newMedia_url;
}
QUrl MediaItemBox::getMediaUrl() const { return mediaUrl_; }

void MediaItemBox::setButtonPlay(bool play) {
  qDebug() << "setButtonPlay" << play;
  ui_->btnPlay->setIcon(
      QIcon(play ? ":/images/circle-play.svg" : ":/images/circle-pause.svg"));
}

void MediaItemBox::setActive(bool active) {
  QString color = active ? "background-color: #E5E5E5" : "";
  this->setStyleSheet(color);
  if (!active) {
    isPlaying_ = false;
    setButtonPlay(!isPlaying_);
  } else {
    emit pause();
  }
}

void MediaItemBox::onClickPlay() {
  qDebug() << "onClickPlay" << isPlaying_;
  setButtonPlay(isPlaying_);
  if (isPlaying_) {
    emit pause();
  } else {
    emit play(mediaUrl_);
  }
  isPlaying_ = !isPlaying_;
}

void MediaItemBox::onClickBtnInfo() {
  qDebug() << "clicked: BtnInfo";
  player_->addFloatTable(ui_->btnInfo, metaDataString_, 1);
}

void MediaItemBox::onClickBtnDel() {
  qDebug() << "clicked: BtnDel";
  emit deleteMedia(mediaUrl_);
}
