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

#include "Player.h"

#include <QFileDialog>
#include <QMediaMetaData>
#include <QTime>

#include "../MediaList/MediaItemBox.h"
#include "../MetaData/MetaDataFloatTable.h"
#include "../UI/ui_Player.h"
#include "ProgressSlider.h"

Player::Player(const QPointer<QWidget>& parent)
    : QWidget(parent), ui_(new Ui::Player) {
  ui_->setupUi(this);

  // initialize audio output and media player
  audioOutput_ = QPointer<QAudioOutput>{new QAudioOutput};
  mediaPlayer_ = QPointer<QMediaPlayer>{new QMediaPlayer};
  audioOutput_->setVolume(sliderVolume());  // necessary?
  mediaPlayer_->setAudioOutput(audioOutput_);
  mediaPlayer_->setVideoOutput(ui_->videoWidget);
  setPlayOrderIcon(PlayOrder::OnlyOnce);

  connect(ui_->open, &QPushButton::clicked,  //
          this, &Player::onClickOpen);
  connect(mediaPlayer_, &QMediaPlayer::positionChanged,  //
          this, &Player::progressing);
  //全屏部分
  connect(ui_->isfullScreen, &QPushButton::clicked,  //
          this, &Player::onClickFullScreen);
  isFullScreen_ = false;
  ui_->videoWidget->installEventFilter(this);
  ui_->videoWidget->setAttribute(Qt::WA_Hover, true);
  frame_ = QPointer<MetaDataFloatTable>{new MetaDataFloatTable()};
  frame_->setWindowFlags(Qt::Popup);
  frame_->setHidden(true);
}

Player::~Player() { delete ui_; }

#pragma region  // region: set values of some widgets

/**
 * @brief Lock and unlock the `loop` checkbox. If the checkbox is locked, it's
 * status should be set to `unchecked` and it couldn't be checked again until it
 * is unlocked.
 * @param lock
 */
void Player::setLoopLock(bool lock) {
  qDebug() << "set loop lock:" << lock;
  if (lock) {
    setPlayOrderIcon(PlayOrder::OnlyOnce);
    ui_->playOrder->setEnabled(false);
  } else {
    ui_->playOrder->setEnabled(true);
  }
}

/**
 * @brief Set the maximum value of the progress slider.
 * @param max
 */
void Player::setProgressSliderMax(const qint32& max) {
  qDebug() << "set progress slider max:" << max;
  ui_->progressSlider->setMaximum(max);
}

/**
 * @brief Set the icon of the `play` button, depending on the parameter.
 * @param play: `true` if media is paused, `false` if media is playing.
 */
void Player::setButtonPlayIcon(bool play) {
  qDebug() << "set button play icon:" << play;
  ui_->play->setIcon(
      QIcon(play ? ":/images/circle-play.svg" : ":/images/circle-pause.svg"));
}

/**
 * @brief Set the icon of the `volume` button, depending on the parameter.
 * @param unmute: `true` if video is muted , `false` if video is unmuted.
 */
void Player::setButtonVolumeIcon(bool unmute) {
  qDebug() << "set button volume icon:" << unmute;
  ui_->volume->setIcon(
      QIcon(unmute ? ":/images/volume-open.svg" : ":/images/volume-close.svg"));
}

/**
 * @brief Set the icon of the `playOrder_` button, depending on the parameter.
 * @param type: play order type.
 */
void Player::setPlayOrderIcon(const PlayOrder& type) {
  QString icon{};
  QString tip{};
  if (type == PlayOrder::OnlyOnce) {
    icon = ":/images/play-onlyOnce.svg";
    tip = tr("only once");
  } else if (type == PlayOrder::InOrder) {
    icon = ":/images/play-inOrder.svg";
    tip = tr("in order");
  } else if (type == PlayOrder::Random) {
    icon = ":/images/play-randomLoop.svg";
    tip = tr("random");
  } else if (type == PlayOrder::SingleLoop) {
    icon = ":/images/play-singleLoop.svg";
    tip = tr("single loop");
  }
  ui_->playOrder->setToolTip(tip);
  ui_->playOrder->setIcon(QIcon(icon));
}

void Player::setIsFullScreenIcon() {
  ui_->isfullScreen->setIcon(QIcon(  // just to wrap the code...
      isFullScreen_ ? ":/images/screen-common.svg"
                    : ":/images/screen-full.svg"));
}

void Player::changeFullScreenStatus() {
  if (isFullScreen_) {
    ui_->groupPlay->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui_->groupPlay->showFullScreen();
    ui_->controlPad->setHidden(true);
  } else {
    ui_->groupPlay->setWindowFlags(Qt::SubWindow);
    ui_->groupPlay->showNormal();
    ui_->controlPad->setHidden(false);
  }
}

void Player::addFloatTable(QPushButton* info, QString str, int posType) {
  MetaDataFloatTable* widget = new MetaDataFloatTable(nullptr);
  widget->setText(str);
  switch (posType) {
    case 0: {  // 顶部
      widget->setPosition(
          info->mapToGlobal(QPoint(0, 0)).x() - widget->width() / 2 +
              info->width() / 2,
          info->mapToGlobal(QPoint(0, 0)).y() - widget->height());
      break;
    };
    case 1: {  // 右方
      widget->setPosition(info->mapToGlobal(QPoint(0, 0)).x() + info->width(),
                          info->mapToGlobal(QPoint(0, 0)).y() -
                              widget->height() / 2 + info->height() / 2);
      break;
    }
  }

  widget->setWindowFlags(Qt::Popup);
  widget->show();
}

void Player::setFrame(QImage image) {
  qDebug() << "set frame_";
  frame_->setImage(
      image.scaled(195, 115, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  frame_->setPosition(
      frame_->x() - frame_->width() / 2,
      ui_->progressSlider->mapToGlobal(QPoint(0, 0)).y() - frame_->height());

  frame_->show();
}

void Player::setFramePos(float x) {
  frame_->setPosition(ui_->progressSlider->mapToGlobal(QPoint(0, 0)).x() +
                          ui_->progressSlider->width() * x,
                      0);
}

void Player::closeFrameShow() {
  frame_->setHidden(true);
  qDebug() << "closeFrame";
}

void Player::addMediaItemBox(QWidget* widget) {
  ui_->scrollMediaListLayout->insertWidget(
      ui_->scrollMediaListLayout->count() - 1, widget);
}

void Player::deleteMediaItemBox(QWidget* widget) {
  widget->setParent(nullptr);
  ui_->scrollMediaListLayout->removeWidget(widget);
}

void Player::addMediaItemSpacerV() {
  ui_->scrollMediaListLayout->addSpacerItem(
      new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

#pragma endregion

#pragma region  // region: get values and status of the player

auto Player::ui() const -> PlayerUiPtr { return ui_; }

auto Player::mediaPlayer() const -> QPointer<QMediaPlayer> {
  return mediaPlayer_;
}

auto Player::audioOutput() const -> QPointer<QAudioOutput> {
  return audioOutput_;
}

bool Player::rewind() const { return ui_->rewind->isChecked(); }

auto Player::duration() const -> qint64 { return mediaPlayer_->duration(); }

auto Player::totalTime() const -> qint64 { return duration() / 1000; }

auto Player::metaData() const -> QMediaMetaData {
  qDebug() << mediaPlayer_->metaData().isEmpty();
  return mediaPlayer_->metaData();
}
auto Player::sliderVolume() const -> qreal {
  // returns a real number from 0 to 1.
  return static_cast<qreal>(ui_->volumeSlider->value()) /
         static_cast<qreal>(ui_->volumeSlider->maximum());
}
auto Player::sliderProgress() const -> qint64 {
  return ui_->progressSlider->value();
}

auto Player::comboBoxRate() const -> qreal {
  // todo
  auto rate_map = QMap<QString, qreal>{
      {tr("0.25x"), 0.25}, {tr("0.5x"), 0.5}, {tr("1x"), 1.0},
      {tr("1.5x"), 1.5},   {tr("2x"), 2.0},
  };
  return rate_map.value(ui_->rate->currentText(), 1.0);
}

bool Player::endOfMedia() const {
  return mediaPlayer_->mediaStatus() == QMediaPlayer::EndOfMedia;
}

#pragma endregion

#pragma region  // region: private slots

/**
 * @brief A slot to keep track of the progression of the media playback and
 * update the time label.
 * @xxx It seems that the `progress_slider` updates every 100 microseconds,
 * therefore there will be ~10 time label updates in one second, of which 90%
 * are useless.
 * @param progress: Current progression of the playback (in microsecond).
 */
void Player::progressing(qint64 progress) {
  if (!ui_->progressSlider->isSliderDown()) {
    ui_->progressSlider->setValue(static_cast<int>(progress));
  }

  // qDebug() << progress;

  //   qDebug() << "progress";

  updateTimeLabel(progress / 1000);
}

/**
 * @brief A slot to handle the click of the `open` button. It opens a Qt file
 * dialog, if user opens a file, it will initialize that media immediately.
 * @todo The format availability of the file should be checked. If a unsupported
 * file is opened, it should be able to give an error message.
 */
void Player::onClickOpen() {
  qDebug() << "clicked: open";
  auto file_dialog = QFileDialog{this};
  file_dialog.setAcceptMode(QFileDialog::AcceptOpen);
  file_dialog.setWindowTitle(tr("Open File"));
  if (file_dialog.exec() == QDialog::Accepted) {
    qDebug() << "open: accepted, " << file_dialog.selectedUrls();
    if (mediaUrl_.isEmpty())
      initMedia(file_dialog.selectedUrls()[0]);
    else
      emit addMedia(file_dialog.selectedUrls()[0]);
  } else {
    qDebug() << "open: rejected";
  }
}

void Player::onClickFullScreen() {
  isFullScreen_ = !isFullScreen_;

  qDebug() << "click:FullScreen:" << isFullScreen_;
  setIsFullScreenIcon();
  changeFullScreenStatus();
}

bool Player::eventFilter(QObject* obj,
                         QEvent* e) {  //不是全屏状态 或者 不是播放框

  if (isFullScreen_ && obj == ui_->videoWidget) {
    if (e->type() == QEvent::HoverMove)  //鼠标移动
    {
      qDebug() << "mouse:move";
      emit showBar();
      ui_->controlPad->setHidden(false);
    }
  }

  // 事件交给上层对话框进行处理
  return QWidget::eventFilter(obj, e);
}

#pragma endregion

#pragma region  // region: private

/**
 * @brief Initialize a media file.
 * @param url: The url of the media file.
 */
void Player::initMedia(const QUrl& url) {
  qDebug() << "init media";
  mediaUrl_ = url;
  mediaPlayer_->setSource(url);
  //  some buttons remain unavailable until a media file is loaded.
  ui_->play->setEnabled(true);
  ui_->stop->setEnabled(true);
  setButtonPlayIcon(true);
  emit addMedia(url);
}

/**
 * @brief Update time label.
 * @param time: Current time (in second).
 */
void Player::updateTimeLabel(qint64 time) {
  auto time_label_text = QString{};
  auto t = static_cast<qint32>(time);
  auto d = static_cast<qint32>(totalTime());
  if (t != 0 || d != 0) {
    auto current = QTime{(t / 3600) % 60, (t / 60) % 60, t % 60, 0};
    auto total = QTime{(d / 3600) % 60, (d / 60) % 60, d % 60, 0};
    auto format = QString{d > 3600 ? "hh:mm:ss" : "mm:ss"};
    time_label_text = current.toString(format) + " / " + total.toString(format);
  }
  ui_->timeLabel->setText(time_label_text);
}

void Player::setMediaUrl(const QUrl& newMedia_url) {
  qDebug() << "setMediaUrl" << newMedia_url;
  if (newMedia_url != mediaUrl_) {
    mediaUrl_ = newMedia_url;
    mediaPlayer_->setSource(newMedia_url);
  }
  playMedia();
}

void Player::playMedia() {
  setButtonPlayIcon(false);
  mediaPlayer_->play();
}

void Player::stopMedia() {
  setButtonPlayIcon(true);
  mediaPlayer_->stop();
}

void Player::pauseMedia() {
  setButtonPlayIcon(true);
  mediaPlayer_->pause();
}

auto Player::state() const -> QMediaPlayer::PlaybackState {
  return mediaPlayer_->playbackState();
}

#pragma endregion
