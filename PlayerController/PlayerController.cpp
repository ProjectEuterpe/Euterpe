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

#include "PlayerController.h"

#include <QJsonObject>

#include "../MetaData/MetaData.h"
#include "../UI/ui_Player.h"

PlayerController::PlayerController(const QPointer<Player> &player) {
  this->player_ = player;
  auto ui = this->player_->ui();
  this->shortcut_ = QPointer<PlayerShortcut>(new PlayerShortcut(player));

  // connect shortcuts
#define shortcut_connection(signal, slot) \
  connect(shortcut_, &PlayerShortcut::signal, this, &PlayerController::slot)
  shortcut_connection(changeVolume, setVolumeValue);
  shortcut_connection(changeProgress, setProgressValue);
#undef shortcut_connection

  // connect clicks on buttons to handlers
#define on_click_connection(sender, slot) \
  connect(ui->sender, &QPushButton::clicked, this, &PlayerController::slot)
  on_click_connection(prev, onClickPrev);
  on_click_connection(play, onClickPlay);
  on_click_connection(play, checkUrl);
  on_click_connection(next, onClickNext);
  on_click_connection(stop, onClickStop);
  on_click_connection(info, onClickInfo);
  on_click_connection(volume, onClickBtnVolume);
#undef on_click_connection

  // connect changes of sliders to handlers
#define on_change_slider_connection(sender, signal, slot) \
  connect(ui->sender, &QSlider::signal, this, &PlayerController::slot)
  on_change_slider_connection(progressSlider, sliderMoved, onChangeProgress);
  on_change_slider_connection(progressSlider, sliderPressed, onChangeProgress);
  on_change_slider_connection(volumeSlider, valueChanged, onChangeVolume);
#undef on_change_slider_connection

  // connect changes of the checkbox and the combobox to handlers
  connect(ui->rewind, &QCheckBox::stateChanged, this,
          &PlayerController::onChangeRate);
  connect(ui->rate, &QComboBox::currentTextChanged, this,
          &PlayerController::onChangeRate);

  connect(player_->mediaPlayer(), &QMediaPlayer::metaDataChanged, this,
          &PlayerController::onChangeMetaData);
  connect(player_->mediaPlayer(), &QMediaPlayer::durationChanged, this,
          &PlayerController::onChangeDuration);
  connect(player_->mediaPlayer(), &QMediaPlayer::metaDataChanged, this,
          &PlayerController::checkUrl);

  // connect signals to Qt media controllers
#define media_control_connection(signal, slot) \
  connect(this, &PlayerController::signal, player_->mediaPlayer(), slot)
  media_control_connection(play, &QMediaPlayer::play);
  media_control_connection(pause, &QMediaPlayer::pause);
  media_control_connection(stop, &QMediaPlayer::stop);

  // todo: connect to MediaList
  connect(this, &PlayerController::prev, mediaList, &MediaList::playPrevMedia);
  connect(this, &PlayerController::next, mediaList, &MediaList::playNextMedia);
  // todo: connect to MetaData
  connect(this, &PlayerController::info, this, []() -> void {});

  media_control_connection(changeRate, &QMediaPlayer::setPlaybackRate);
  media_control_connection(changeProgress, &QMediaPlayer::setPosition);

  connect(this, &PlayerController::stop, ui->videoWidget,
          QOverload<>::of(&QVideoWidget::update));
  connect(this, &PlayerController::changeVolume, player_->audioOutput(),
          &QAudioOutput::setVolume);
  connect(player_->mediaPlayer(), &QMediaPlayer::mediaStatusChanged, this,
          &PlayerController::atEnd);
#undef media_control_connection

  //全屏时工具栏计时器
  showBarTimer = QPointer<QTimer>{new QTimer()};
  connect(showBarTimer, &QTimer::timeout, this, &PlayerController::onTimerEnd);
  connect(player_, &Player::showBar, this, &PlayerController::onTimerStart);

  frameData = QPointer<GetFrameData>{new GetFrameData(player_)};

  //帧展示
  connect(frameData, &GetFrameData::doneGetFrame, this,
          &PlayerController::showFrameData);
  connect(player_, &Player::showFrameSignal,
   this,   &PlayerController::onProgressMouseOn);

  // 初始化媒体库列表
  initMediaList();

  // 新导入媒体 addMedia
  connect(player_, &Player::addMedia, this, &PlayerController::addMediaItem);
  // 切换媒体
  connect(mediaList, &MediaList::changeCurrentMedia, this,
          &PlayerController::onChangeCurrMedia);
  // 播放暂停
  connect(mediaList, &MediaList::play, player_, &Player::playMedia);
  connect(mediaList, &MediaList::play, this, &PlayerController::checkUrl);
  connect(mediaList, &MediaList::pause, player_, &Player::pauseMedia);
  connect(mediaList, &MediaList::stop, player_, &Player::stopMedia);
  // 播放顺序控制
  connect(ui->playOrder, &QPushButton::clicked, mediaList,
          &MediaList::onChangePlayOrder);

  // 音视频列表播放切换
  // connect(player_->mediaPlayer(), &QMediaPlayer::sourceChanged,
  // this,&PlayerController::checkUrl);
  connect(mediaList, &MediaList::endOfMediaList, this,
          &PlayerController::showInitWidget);
}

//快捷键使用
void PlayerController::setVolumeValue(const int &add) {
  qDebug() << "shortcut: change volume";
  float nowVolume = player_->ui()->volumeSlider->value();
  player_->ui()->volumeSlider->setFocus();
  player_->ui()->volumeSlider->setValue(nowVolume + add);
}
//进度微调
void PlayerController::setProgressValue(const int &add) {
  bool playing = player_->state() == QMediaPlayer::PlayingState;
  if (playing) player_->mediaPlayer()->pause();
  qDebug() << "beforeProgressPos:" << player_->mediaPlayer()->position();
  qint64 target =
      frameData->GetTargetFrameTime(player_->mediaPlayer()->position(), add);
  if (target < 0)
    target = 0;
  else if (target > player_->duration())
    target = player_->duration();
  player_->ui()->progressSlider->setValue(target);
  player_->mediaPlayer()->setPosition(target);
  qDebug() << "afterProgressPos:" << player_->mediaPlayer()->position();
  if (playing) player_->mediaPlayer()->play();
}

auto PlayerController::getMetaData(const QMediaMetaData::Key &key) -> QVariant {
  return player_->metaData().value(key);
}

void PlayerController::initMediaList() {
  player_->addMediaItemSpacerV();
  mediaList = QPointer<MediaList>{new MediaList(player_)};

  auto database_ = mediaList->database_;
  // get data from database...
  for (const auto &row : database_->table()) {
    // qDebug() << QUrl("file://" + row->mediaPath);
    player_->initMedia(QUrl("file://" + row->mediaPath));
    auto xx = MetaData(row->metaData);
    mediaList->addMediaItemBox(QUrl("file://" + row->mediaPath),
                               xx.get("Album title").toString(),
                               xx.get("Title").toString());
  }
}
//音视频控制
void PlayerController::checkUrl() {  //貌似没有值，待实现

  QString type = getMetaData(QMediaMetaData::MediaType).toString();
  qDebug() << getMetaData(QMediaMetaData::MediaType).toString()
           << "显示音频或视频";
  QUrl url = player_->url();
  const QFileInfo fileInfo(url.toLocalFile());
  int type1 = fileInfo.suffix().compare("mp3", Qt::CaseInsensitive);
  int type2 = fileInfo.suffix().compare("wav", Qt::CaseInsensitive);
  int type3 = fileInfo.suffix().compare("mid", Qt::CaseInsensitive);
  int type4 = fileInfo.suffix().compare("aif", Qt::CaseInsensitive);

  // playAudio();

  if (type1 == 0 || type2 == 0 || type == "audio" || type3 == 0 || type4 == 0) {
    playAudio();
  } else
    playVideo();
}

void PlayerController::playVideo() {
  qDebug() << "play video";
  shortcut_->playVideo();
  // player_->ui_()->progress_slider->setIsVideo(true);
  frameData->setIsVideo(true);
  player_->ui()->stacked_widget->setCurrentWidget(player_->ui()->videoWidget);
}

void PlayerController::playAudio() {
  qDebug() << "play audio";
  shortcut_->playAudio();
  // player_->ui_()->progress_slider->setIsVideo(false);
  frameData->setIsVideo(false);
  //专辑图片展示
  auto metadata = player_->metaData();
  QString name = metadata.value(QMediaMetaData::Title).toString();
  QString artist = metadata.value(QMediaMetaData::Author).toString();
  QString album = getMetaData(QMediaMetaData::AlbumTitle).toString();
  player_->ui()->name->setText(name);
  player_->ui()->album->setText(album);
  player_->ui()->artist->setText(artist);
  QImage img = metadata.value(QMediaMetaData::ThumbnailImage).value<QImage>();
  QPixmap pixmap = QPixmap::fromImage(img);
  QPixmap fitpixmap =
      pixmap.scaled(150, 150, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  player_->ui()->img->setScaledContents(true);
  player_->ui()->img->setPixmap(fitpixmap);

  if (name == "") player_->ui()->name->setText("Untitled");
  if (artist == "") player_->ui()->artist->setText("Unknown");
  if (album == "") player_->ui()->album->setText("Unknown");
  if (img.isNull()) {
      QImage *img = new QImage;
    img->load(":/images/2.jpg");
    QPixmap pixmap = QPixmap::fromImage(*img);
    QPixmap fitpixmap = pixmap.scaled(150, 150, Qt::IgnoreAspectRatio,
                                      Qt::SmoothTransformation);
    player_->ui()->img->setScaledContents(true);
    player_->ui()->img->setPixmap(fitpixmap);
  }
  player_->ui()->stacked_widget->setCurrentWidget(player_->ui()->audioWidget);
}

void PlayerController::addMediaItem(QUrl url) {
  if (url.isEmpty()) return;
  mediaList->importMedia(url);
}

void PlayerController::onChangeMetaData() { qDebug() << "onChangeMetaData"; }

void PlayerController::onChangeDuration() {
  qDebug() << "onChangeDuration";
  player_->setProgressSliderMax(static_cast<int>(player_->duration()));
}

#pragma region  // region: controller slots

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
  bool playing = player_->state() == QMediaPlayer::PlayingState;
  emit playing ? pause() : play();
  player_->setButtonPlayIcon(playing);
  mediaList->playStop(playing);
  // default values of artist and title will be "Untitled" and "V/A"
  auto metadata = player_->metaData();
  auto artist = metadata.value(QMediaMetaData::AlbumArtist).toString();
  auto title = metadata.value(QMediaMetaData::Title).toString();
  if (artist == "") {
    artist = metadata.value(QMediaMetaData::ContributingArtist).toString();
  }  // todo!

  player_->setWindowTitle(QString("%1 - %2").arg(
      artist == "" ? "V/A" : artist, title == "" ? "Untitled" : title));
}

/**
 * @brief Handler called when the `stop` button is clicked. The label of `play`
 * button should be reset to "PLAY".
 */
void PlayerController::onClickStop() {
  qDebug() << "clicked: stop";
  emit stop();
  player_->setButtonPlayIcon(true);
}

/**
 * @brief Handler called when the `prev` button is clicked.
 */
void PlayerController::onClickPrev() {
  qDebug() << "clicked: prev";
  emit prev();
}

/**
 * @brief Handler called when the `next` button is clicked.
 */
void PlayerController::onClickNext() {
  qDebug() << "clicked: next";
  emit next();
}

/**
 * @brief Handler called when the `info` button is clicked. It gets the metaData
 * of the media.
 * @todo It just print the metaData of the media file to debug stream for now.
 */
void PlayerController::onClickInfo() {
  qDebug() << "clicked: info";
  qDebug() << "media metaData:";
  auto metaData = player_->metaData();
  auto m = MetaData(metaData);
  player_->addFloatTable(player_->ui()->info, m.toPrettyString());
  emit info();
}

/**
 * @brief Handler called when the value of the progress slider is changed.
 */
void PlayerController::onChangeProgress() {
  qDebug() << "changed: progress";
  emit changeProgress(player_->sliderProgress());
}

/**
 * @brief Handler called when the value of the volume slider is changed.
 * If the previous volume or current volume is 0, change then volume icon
 * matching the current volume.
 */
void PlayerController::onChangeVolume() {
  qDebug() << "changed: volume";
  float volumeCur = player_->sliderVolume();
  float volumePre = player_->audioOutput()->volume();
  if (volumeCur && !volumePre) {
    player_->setButtonVolumeIcon(1);
  } else if (!volumeCur && volumePre) {
    player_->setButtonVolumeIcon(0);
  }
  emit changeVolume(volumeCur);
}

/**
 * @brief Handler called when the value of `rate` combobox is changed.
 */
void PlayerController::onChangeRate() {
  qDebug() << "changed: rate";
  player_->setLoopLock(player_->rewind());
  emit changeRate((player_->rewind() ? -1.0 : 1.0) * player_->comboBoxRate());
}

/*!
 * @brief Handler called when the media is at it's end. If the `loop` checkbox
 * is checked, it emits a `play()` signal to replay the media, otherwise it
 * emits `stop()`.
 */
void PlayerController::atEnd() {
  if (player_->endOfMedia()) {
    qDebug() << "end of media";
    //    emit player_->loop() ? play() : stop();
    //    player_->setButtonPlayIcon(!player_->loop());
    mediaList->onNextMedia();
  }
  if (player_->mediaPlayer()->mediaStatus() == QMediaPlayer::LoadedMedia) {
    qDebug() << "LOADED!";
    mediaList->insertToDatabase(player_->mediaPlayer()->source());
  }
}

void PlayerController::onTimerStart() {
  qDebug() << "TimeStart";
  showBarTimer->stop();
  showBarTimer->start(5000);
}

void PlayerController::onTimerEnd() {
  //隐藏
  qDebug() << "TimeOut";
  if (player_->isFullScreen())
      player_->ui()->controlPad->setHidden(true);
  showBarTimer->stop();
}

void PlayerController::onProgressMouseOn(const double per) {
  //获取目标时间
  int target = per * getMetaData(QMediaMetaData::Duration).toInt();
  qDebug() << "hover time:" << target;
  player_->setFramePos(per);
  frameData->GetTargetFrameImage(target);
}

void PlayerController::showFrameData(QImage image) {
  player_->setFrame(image);
}

/**
 * @brief Handler called when the value of `btn_volume` button is clicked.
 * It set the volume 0 or previous value.
 */
void PlayerController::onClickBtnVolume() {
  qDebug() << "clicked: btn_volume";
  float volume = player_->audioOutput()->volume();
  if (volume) {
    player_->setButtonVolumeIcon(false);
    emit changeVolume(0);
  } else {
    player_->setButtonVolumeIcon(true);
    emit changeVolume(player_->sliderVolume());
  }
}

void PlayerController::onChangeCurrMedia(QUrl url) {
  player_->setMediaUrl(url);
}

void PlayerController::showInitWidget() {
  player_->ui()->stacked_widget->setCurrentWidget(player_->ui()->initWidget);
}
#pragma endregion
