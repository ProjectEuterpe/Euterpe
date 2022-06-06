/**
 * @file PlayerController.cpp
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

#include "PlayerController.h"

PlayerController::PlayerController(const QPointer<Player> &player)
    : player_(player),
      shortcut_(QPointer<PlayerShortcut>(new PlayerShortcut(player))),
      showBarTimer_(QPointer<QTimer>(new QTimer())),
      frameData_(QPointer<FrameData>(new FrameData(player_))) {
  auto ui = this->player_->ui();
  this->initMediaList();

  // connect shortcuts
#define SHORTCUT_CONNECTION(signal, slot)                 \
  connect(this->shortcut_, &PlayerShortcut::signal, this, \
          &PlayerController::slot)
  SHORTCUT_CONNECTION(changeVolume, setVolumeValue);
  SHORTCUT_CONNECTION(changeProgress, setProgressValue);
#undef SHORTCUT_CONNECTION

  // connect clicks on buttons to handlers
#define ON_CLICK_CONNECTION(sender, slot) \
  connect(ui->sender, &QPushButton::clicked, this, &PlayerController::slot)
  ON_CLICK_CONNECTION(prev, onClickPrev);
  ON_CLICK_CONNECTION(play, onClickPlay);
  ON_CLICK_CONNECTION(play, checkUrl);
  ON_CLICK_CONNECTION(next, onClickNext);
  ON_CLICK_CONNECTION(stop, onClickStop);
  ON_CLICK_CONNECTION(info, onClickInfo);
  ON_CLICK_CONNECTION(volume, onClickMute);
#undef ON_CLICK_CONNECTION

  // connect changes of sliders to handlers
#define ON_CHANGE_SLIDER_CONNECTION(sender, signal, slot) \
  connect(ui->sender, &QSlider::signal, this, &PlayerController::slot)
  ON_CHANGE_SLIDER_CONNECTION(progressSlider, sliderMoved, onChangeProgress);
  ON_CHANGE_SLIDER_CONNECTION(progressSlider, sliderPressed, onChangeProgress);
  ON_CHANGE_SLIDER_CONNECTION(volumeSlider, valueChanged, onChangeVolume);
#undef ON_CHANGE_SLIDER_CONNECTION

  // connect changes of the checkbox and the combobox to handlers
  connect(ui->rewind, &QCheckBox::stateChanged, this,
          &PlayerController::onChangeRate);
  connect(ui->rate, &QComboBox::currentTextChanged, this,
          &PlayerController::onChangeRate);

  // connect media player
#define MEDIA_PLAYER_CONNECTION(signal, slot)                        \
  connect(this->player_->mediaPlayer(), &QMediaPlayer::signal, this, \
          &PlayerController::slot)
  MEDIA_PLAYER_CONNECTION(metaDataChanged, checkUrl);
  MEDIA_PLAYER_CONNECTION(metaDataChanged, onChangeMetaData);
  MEDIA_PLAYER_CONNECTION(durationChanged, onChangeDuration);
  MEDIA_PLAYER_CONNECTION(mediaStatusChanged, onMediaStatusChanged);
#undef MEDIA_PLAYER_CONNECTION

  // connect signals to Qt media controllers
#define MEDIA_CONTROLLER_CONNECTION(signal, slot) \
  connect(this, &PlayerController::signal, this->player_->mediaPlayer(), slot)
  MEDIA_CONTROLLER_CONNECTION(play, &QMediaPlayer::play);
  MEDIA_CONTROLLER_CONNECTION(pause, &QMediaPlayer::pause);
  MEDIA_CONTROLLER_CONNECTION(stop, &QMediaPlayer::stop);
  MEDIA_CONTROLLER_CONNECTION(changeRate, &QMediaPlayer::setPlaybackRate);
  MEDIA_CONTROLLER_CONNECTION(changeProgress, &QMediaPlayer::setPosition);
#undef MEDIA_CONTROLLER_CONNECTION

  connect(this, &PlayerController::sequence, this->mediaList_,
          &MediaList::playPrevNextMedia);
  connect(this, &PlayerController::stop, ui->videoWidget,
          QOverload<>::of(&QVideoWidget::update));
  connect(this, &PlayerController::changeVolume, this->player_->audioOutput(),
          &QAudioOutput::setVolume);

  connect(this->player_, &Player::showBar, this,
          &PlayerController::onTimerStart);
  connect(this->showBarTimer_, &QTimer::timeout, this,
          &PlayerController::onTimerEnd);

  connect(this->frameData_, &FrameData::getFrameDone, this,
          &PlayerController::showFrameData);
  connect(this->player_, &Player::showFrame, this,
          &PlayerController::onProgressMouseOn);
  connect(this->player_, &Player::addMedia, this,
          &PlayerController::addMediaItem);

#define MEDIA_LIST_CONNECTION(signal, slot) \
  connect(this->mediaList_, &MediaList::signal, this->player_, &Player::slot);
  MEDIA_LIST_CONNECTION(play, setMediaUrl);
  MEDIA_LIST_CONNECTION(play, playMedia);
  MEDIA_LIST_CONNECTION(pause, pauseMedia);
  MEDIA_LIST_CONNECTION(stop, stopMedia);
  connect(this->mediaList_, &MediaList::play, this,
          &PlayerController::checkUrl);
  connect(this->mediaList_, &MediaList::changeCurrentMedia, this,
          &PlayerController::onChangeCurrentMedia);
  connect(this->mediaList_, &MediaList::endOfMediaList, this,
          &PlayerController::showInitWidget);
  connect(ui->playOrder, &QPushButton::clicked, this->mediaList_,
          &MediaList::onChangePlayOrder);
#undef MEDIA_LIST_CONNECTION
}

#pragma region private

auto PlayerController::getMetaData(const QMediaMetaData::Key &key) const
    -> QVariant {
  return this->player_->metaData().value(key);
}

void PlayerController::initMediaList() {
  this->player_->addMediaItemSpace();
  this->mediaList_ = QPointer<MediaList>(new MediaList(player_));
  // get data from database...
  for (const auto &row : this->mediaList_->databaseTable()) {
    auto url = QUrl::fromLocalFile(row->mediaPath);
    auto check = QFileInfo(QString::fromLocal8Bit(url.path().toStdString()));
    if (!check.exists() || !check.isFile()) {
      auto result =
          QMessageBox::question(this, "File does not exist...",
                                url.path() + " does not exist. Remove it?");
      if (result == QMessageBox::Yes) {
        qDebug() << "remove:" << url;
        emit this->player_->remove(url);
        continue;
      } else {
        qDebug() << "But there is nothing else you can do!";
      }
    }
    this->player_->initMedia(url);
    auto metaData = MetaData(row->metaData);
    this->mediaList_->addMediaItemBox(
        url, metaData.get("Contributing artist").toString(),
        metaData.get("Title").toString(), url.fileName());
  }
}

void PlayerController::addMediaItem(const QUrl &url) const {
  this->mediaList_->importMedia(url);
}

void PlayerController::playVideo() {
  this->shortcut_->playVideo();
  this->frameData_->setIsVideo(true);
  this->player_->ui()->stackedWidget->setCurrentWidget(
      this->player_->ui()->videoWidget);
  this->setTitle();
}

void PlayerController::playAudio() {
  this->shortcut_->playAudio();
  this->frameData_->setIsVideo(false);
  // album cover
  auto metadata = player_->metaData();
  auto name = metadata.value(QMediaMetaData::Title).toString();
  auto artist = metadata.value(QMediaMetaData::Author).toString();
  auto album = metadata.value(QMediaMetaData::AlbumTitle).toString();
  this->player_->ui()->name->setText(name);
  this->player_->ui()->album->setText(album);
  this->player_->ui()->artist->setText(artist);
  auto img = metadata.value(QMediaMetaData::ThumbnailImage).value<QImage>();
  auto pixmap = QPixmap::fromImage(img);
  if (!pixmap.isNull()) {
    auto fitPixmap = pixmap.scaled(150, 150, Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation);
    this->player_->ui()->img->setPixmap(fitPixmap);
  }
  this->player_->ui()->img->setScaledContents(true);

  if (name.isEmpty()) {
    this->player_->ui()->name->setText(tr("Untitled"));
  }
  if (artist.isEmpty()) {
    this->player_->ui()->artist->setText(tr("V/A"));
  }
  if (album.isEmpty()) {
    this->player_->ui()->album->setText(tr("Untitled"));
  }
  if (img.isNull()) {
    auto newImg = new QImage;
    newImg->load(":/images/2.jpg");
    auto newPixmap = QPixmap::fromImage(*newImg);
    if (!newPixmap.isNull()) {
      auto newFitPixmap = newPixmap.scaled(150, 150, Qt::IgnoreAspectRatio,
                                           Qt::SmoothTransformation);
      this->player_->ui()->img->setPixmap(newFitPixmap);
    }
    this->player_->ui()->img->setScaledContents(true);
  }
  this->player_->ui()->stackedWidget->setCurrentWidget(
      this->player_->ui()->audioWidget);
  this->setTitle();
}

void PlayerController::setTitle() const {
  for (const auto &row :
       this->mediaList_->findInDatabase(this->player_->url())) {
    auto metaData = MetaData(row->metaData);
    auto artist = metaData.get("Contributing artist").toString();
    auto title = metaData.get("Title").toString();
    this->player_->setWindowTitle(
        QString("%1 - %2").arg(artist.isEmpty() ? tr("V/A") : artist,
                               title == "" ? tr("Untitled") : title));
  }
}

#pragma endregion

#pragma region slots

/**
 * @brief Handler called when the `play` button is clicked. It emits signal
 * `pause()`/`play` and change the label of `play` button to "PLAY"/"PAUSE"
 * depending on current playing state. It also set main window title to artist
 * - title format.
 * @xxx It ineffectively set the total progress of the media each time `play`
 * is clicked. The fallback of artist and title name is inelegant.
 */
void PlayerController::onClickPlay() {
  qDebug() << "clicked: play";
  bool playing = this->player_->state() == QMediaPlayer::PlayingState;
  emit playing ? this->pause() : this->play();
  this->player_->setButtonPlayIcon(playing);
  this->mediaList_->playStop(playing);
}

/**
 * @brief Handler called when the `stop` button is clicked. The label of `play`
 * button should be reset to "PLAY".
 */
void PlayerController::onClickStop() {
  qDebug() << "clicked: stop";
  emit this->stop();
  this->player_->setButtonPlayIcon(true);
}

/**
 * @brief Handler called when the `prev` button is clicked.
 */
void PlayerController::onClickPrev() {
  qDebug() << "clicked: prev";
  emit this->sequence(Sequence::Prev);
}

/**
 * @brief Handler called when the `next` button is clicked.
 */
void PlayerController::onClickNext() {
  qDebug() << "clicked: next";
  emit this->sequence(Sequence::Next);
}

/**
 * @brief Handler called when the `info` button is clicked. It gets the metaData
 * of the media.
 * @todo It just print the metaData of the media file to debug stream for now.
 */
void PlayerController::onClickInfo() {
  qDebug() << "clicked: info";
  auto metaData = this->player_->metaData();
  this->player_->addFloatTable(this->player_->ui()->info,
                               MetaData(metaData).toPrettyString());
  emit this->info();
}

/**
 * @brief Handler called when the value of the progress slider is changed.
 */
void PlayerController::onChangeProgress() {
  qDebug() << "changed: progress";
  emit this->changeProgress(this->player_->sliderProgress());
}

/**
 * @brief Handler called when the value of the volume slider is changed.
 * If the previous volume or current volume is 0, change then volume icon
 * matching the current volume.
 */
void PlayerController::onChangeVolume() {
  qDebug() << "changed: volume";
  auto currentVolume = this->player_->sliderVolume();
  auto previousVolume = this->player_->audioOutput()->volume();
  if (currentVolume != 0 && previousVolume == 0) {
    this->player_->setButtonVolumeIcon(true);
  } else if (currentVolume == 0 && previousVolume != 0) {
    this->player_->setButtonVolumeIcon(false);
  }
  emit this->changeVolume(currentVolume);
}

/**
 * @brief Handler called when the value of `rate` combobox is changed.
 */
void PlayerController::onChangeRate() {
  qDebug() << "changed: rate";
  this->player_->setLoopLock(this->player_->rewind());
  emit this->changeRate((this->player_->rewind() ? -1.0 : 1.0) *
                        this->player_->comboBoxRate());
}

/**
 * @brief Handler called when the value of `volume` button is clicked.
 * It set the volume 0 or previous value.
 */
void PlayerController::onClickMute() {
  auto volume = this->player_->audioOutput()->volume();
  if (volume != 0) {
    this->player_->setButtonVolumeIcon(false);
    emit this->changeVolume(0);
  } else {
    this->player_->setButtonVolumeIcon(true);
    emit this->changeVolume(this->player_->sliderVolume());
  }
}

/**
 * @brief Handler called when the media is at it's end. If the `loop` checkbox
 * is checked, it emits a `play()` signal to replay the media, otherwise it
 * emits `stop()`.
 */
void PlayerController::onMediaStatusChanged() {
  if (this->player_->endOfMedia()) {
    this->mediaList_->onNextMedia();
  } else if (this->player_->mediaPlayer()->mediaStatus() ==
             QMediaPlayer::LoadedMedia) {
    this->mediaList_->insertToDatabase(this->player_->mediaPlayer()->source());
  }
}

/**
 * @brief
 */
void PlayerController::onTimerStart() {
  this->showBarTimer_->stop();
  this->showBarTimer_->start(5000);
}

/**
 * @brief
 */
void PlayerController::onTimerEnd() {
  if (this->player_->isFullScreen()) {
    this->player_->ui()->controlPad->setHidden(true);
  }
  this->showBarTimer_->stop();
}

/**
 * @brief
 */
void PlayerController::onChangeMetaData() {}

/**
 * @brief
 */
void PlayerController::onChangeDuration() {
  this->player_->setProgressSliderMax(
      static_cast<qint32>(this->player_->duration()));
}

/**
 * @brief
 * @param percent
 */
void PlayerController::onProgressMouseOn(const qfloat16 &percent) {
  auto target = static_cast<qint64>(
      percent * this->getMetaData(QMediaMetaData::Duration).toInt());
  this->player_->setFramePos(percent);
  this->frameData_->targetFrameImage(target);
}

/**
 * @brief
 * @param url
 */
void PlayerController::onChangeCurrentMedia(const QUrl &url) {
  this->player_->setMediaUrl(url);
  this->checkUrl();
}

/**
 * @brief
 */
void PlayerController::checkUrl() {
  auto type = this->getMetaData(QMediaMetaData::MediaType).toString();
  auto url = this->player_->url();
  auto fileInfo = QFileInfo(url.toLocalFile());
  auto type1 = fileInfo.suffix().compare("mp3", Qt::CaseInsensitive);
  auto type2 = fileInfo.suffix().compare("wav", Qt::CaseInsensitive);
  auto type3 = fileInfo.suffix().compare("mid", Qt::CaseInsensitive);
  auto type4 = fileInfo.suffix().compare("aif", Qt::CaseInsensitive);
  (type == "audio" || type1 == 0 || type2 == 0 || type3 == 0 || type4 == 0)
      ? this->playAudio()
      : this->playVideo();
}

/**
 * @brief
 * @param image
 */
void PlayerController::showFrameData(const QImage &image) {
  this->player_->setFrame(image);
}

/**
 * @brief
 */
void PlayerController::showInitWidget() {
  this->player_->ui()->stackedWidget->setCurrentWidget(
      this->player_->ui()->initWidget);
}

/**
 * @brief
 * @param delta
 */
void PlayerController::setVolumeValue(const qint32 &delta) {
  qDebug() << "shortcut: change volume";
  auto volume = this->player_->ui()->volumeSlider->value();
  this->player_->ui()->volumeSlider->setFocus();
  this->player_->ui()->volumeSlider->setValue(volume + delta);
}

/**
 * @brief
 * @param delta
 */
void PlayerController::setProgressValue(const qint32 &delta) {
  bool playing = this->player_->state() == QMediaPlayer::PlayingState;
  if (playing) {
    this->player_->mediaPlayer()->pause();
  }
  auto target = this->frameData_->getTargetFrameTime(
      this->player_->mediaPlayer()->position(), delta);
  if (target < 0) {
    target = 0;
  } else if (target > this->player_->duration()) {
    target = this->player_->duration();
  }
  this->player_->ui()->progressSlider->setValue(static_cast<qint32>(target));
  this->player_->mediaPlayer()->setPosition(target);
  if (playing) {
    this->player_->mediaPlayer()->play();
  }
}

#pragma endregion
