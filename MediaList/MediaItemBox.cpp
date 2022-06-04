/**
 * @file MediaItemBox.cpp
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

#include "MediaItemBox.h"

#include "../UI/ui_MediaItemBox.h"

#pragma region public

MediaItemBox::MediaItemBox(const QPointer<Player> &parent)
    : QGroupBox(parent.data()),
      ui_(new Ui::MediaItemBox),
      player_(parent),
      isPlaying_(false) {
  this->ui_->setupUi(this);

#define CONNECTION_BOX(sender, slot) \
  connect(this->ui_->sender, &QPushButton::clicked, this, &MediaItemBox::slot)
  CONNECTION_BOX(play, onClickPlay);
  CONNECTION_BOX(info, onClickInfo);
  CONNECTION_BOX(del, onClickDel);
#undef CONNECTION_BOX
}

MediaItemBox::~MediaItemBox() { delete this->ui_; }

void MediaItemBox::setMediaTitle(const QString &title) {
  auto text = title.isEmpty() ? tr("Unknown Title") : title;
  this->ui_->textTitle->setText(text);
  this->ui_->textTitle->setToolTip(text);
}

void MediaItemBox::setMediaArtist(const QString &artist) {
  auto text = artist.isEmpty() ? tr("V/A") : artist;
  this->ui_->textArtist->setText(text);
  this->ui_->textArtist->setToolTip(text);
}

[[maybe_unused]] void MediaItemBox::setImage(const QImage &img) {}

void MediaItemBox::setMetaData(const QMediaMetaData &data) {
  this->metaData_ = QSharedPointer<MetaData>(new MetaData(data));
  this->metaDataString_ = this->metaData_->toPrettyString();
}

void MediaItemBox::setMediaUrl(const QUrl &newMedia_url) {
  this->mediaUrl_ = newMedia_url;
}

void MediaItemBox::setButtonPlay(bool play) {
  this->ui_->play->setIcon(
      QIcon(play ? ":/images/circle-play.svg" : ":/images/circle-pause.svg"));
}

void MediaItemBox::setActive(bool active) {
  auto color = active ? "background-color: #E5E5E5" : "";
  this->setStyleSheet(color);
  if (!active) {
    this->isPlaying_ = false;
    this->setButtonPlay(!isPlaying_);
  } else {
    emit this->pause();
  }
}

QUrl MediaItemBox::getMediaUrl() const { return this->mediaUrl_; }

#pragma endregion

#pragma region slots

void MediaItemBox::onClickPlay() {
  this->setButtonPlay(this->isPlaying_);
  emit this->isPlaying_ ? this->pause() : this->play(this->mediaUrl_);
  this->isPlaying_ = !this->isPlaying_;
}

void MediaItemBox::onClickInfo() {
  this->player_->addFloatTable(this->ui_->info, this->metaDataString_,
                               Position::RIGHT);
}

void MediaItemBox::onClickDel() { emit this->removeMedia(this->mediaUrl_); }

#pragma endregion
