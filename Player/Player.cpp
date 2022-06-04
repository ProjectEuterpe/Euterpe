/**
 * @file Player.cpp
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

#include "Player.h"

#include "../UI/ui_Player.h"

#pragma region public

Player::Player(const QPointer<QWidget>& parent)
    : QWidget(parent),
      ui_(new Ui::Player),
      isFullScreen_(false),
      showFrame_(false),
      audioOutput_(QPointer<QAudioOutput>(new QAudioOutput)),
      mediaPlayer_(QPointer<QMediaPlayer>(new QMediaPlayer)),
      frame_(QPointer<MetaDataFloatTable>(new MetaDataFloatTable())) {
  this->ui_->setupUi(this);

  // initialize audio output and media player
  this->audioOutput_->setVolume(
      static_cast<float>(this->sliderVolume()));  // necessary?
  this->mediaPlayer_->setAudioOutput(this->audioOutput_);
  this->mediaPlayer_->setVideoOutput(this->ui_->videoWidget);

  this->setPlayOrderIcon(PlayOrder::OnlyOnce);
  connect(this->ui_->open, &QPushButton::clicked, this, &Player::onClickOpen);
  connect(this->mediaPlayer_, &QMediaPlayer::positionChanged, this,
          &Player::progressing);
  connect(this->ui_->isfullScreen, &QPushButton::clicked, this,
          &Player::onClickFullScreen);

  this->ui_->videoWidget->installEventFilter(this);
  this->ui_->videoWidget->setAttribute(Qt::WA_Hover, true);
  this->ui_->progressSlider->installEventFilter(this);
  this->ui_->progressSlider->setAttribute(Qt::WA_Hover, true);
  this->ui_->stackedWidget->setCurrentWidget(this->ui_->initWidget);

  this->frame_->setWindowFlags(Qt::Popup | Qt::Tool | Qt::FramelessWindowHint);
  this->frame_->setHidden(true);

  auto img = new QImage;
  img->load(":/images/1.jpg");
  auto pixmap = QPixmap::fromImage(*img);
  auto fitPixmap =
      pixmap.scaled(311, 231, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  this->ui_->label_2->setScaledContents(true);
  this->ui_->label_2->setPixmap(fitPixmap);
}

Player::~Player() { delete this->ui_; }

/**
 * @brief Initialize a media file.
 * @param url: The url of the media file.
 */
void Player::initMedia(const QUrl& url) {
  qDebug() << "init media:" << url;
  this->mediaUrl_ = url;
  this->mediaPlayer_->setSource(url);
  // some buttons remain unavailable until a media file is loaded.
  this->ui_->play->setEnabled(true);
  this->ui_->stop->setEnabled(true);
  this->ui_->prev->setEnabled(true);
  this->ui_->next->setEnabled(true);
  this->setButtonPlayIcon(true);
  this->setButtonPrevIcon();
  this->setButtonNextIcon();
  emit this->addMedia(url);
}

/**
 * @brief Lock and unlock the `loop` checkbox. If the checkbox is locked, it's
 * status should be set to `unchecked` and it couldn't be checked again until it
 * is unlocked.
 * @param lock
 */
void Player::setLoopLock(bool lock) {
  if (lock) {
    this->setPlayOrderIcon(PlayOrder::OnlyOnce);
    this->ui_->playOrder->setEnabled(false);
  } else {
    this->ui_->playOrder->setEnabled(true);
  }
}

/**
 * @brief Set the maximum value of the progress slider.
 * @param max
 */
void Player::setProgressSliderMax(const qint32& max) {
  this->ui_->progressSlider->setMaximum(max);
}

/**
 * @brief Set the icon of the `play` button, depending on the parameter.
 * @param play: `true` if media is paused, `false` if media is playing.
 */
void Player::setButtonPlayIcon(bool play) {
  this->ui_->play->setIcon(
      QIcon(play ? ":/images/circle-play.svg" : ":/images/circle-pause.svg"));
}

/**
 * @brief Set the icon of the `prev` button.
 */
void Player::setButtonPrevIcon() {
  this->ui_->prev->setIcon(QIcon(":/images/step-previous.svg"));
}

/**
 * @brief Set the icon of the `next` button.
 */
void Player::setButtonNextIcon() {
  this->ui_->next->setIcon(QIcon(":/images/step-next.svg"));
}

/**
 * @brief Set the icon of the `volume` button, depending on the parameter.
 * @param unmute: `true` if video is muted , `false` if video is unmuted.
 */
void Player::setButtonVolumeIcon(bool unmute) {
  this->ui_->volume->setIcon(
      QIcon(unmute ? ":/images/volume-open.svg" : ":/images/volume-close.svg"));
}

/**
 * @brief Set the icon of the `playOrder_` button, depending on the parameter.
 * @param type: play order type.
 */
void Player::setPlayOrderIcon(const PlayOrder& type) {
  auto icon = QString();
  auto tip = QString();
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
  this->ui_->playOrder->setToolTip(tip);
  this->ui_->playOrder->setIcon(QIcon(icon));
}

void Player::setIsFullScreenIcon() {
  this->ui_->isfullScreen->setIcon(QIcon(  // just to wrap the code...
      this->isFullScreen_ ? ":/images/screen-common.svg"
                          : ":/images/screen-full.svg"));
}

void Player::setFrame(const QImage& image) {
  this->frame_->setImage(
      image.scaled(195, 115, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  this->frame_->setPosition(
      this->frame_->x() - this->frame_->width() / 2,
      this->ui_->progressSlider->mapToGlobal(QPoint(0, 0)).y() -
          this->frame_->height());
  this->frame_->show();
  this->showFrame_ = true;
}

void Player::setFramePos(const qreal& x) {
  this->frame_->setPosition(
      this->ui_->progressSlider->mapToGlobal(QPoint(0, 0)).x() +
          this->ui_->progressSlider->width() * x,
      0);
}

void Player::setMediaUrl(const QUrl& url) {
  if (url != this->mediaUrl_) {
    this->mediaUrl_ = url;
    this->mediaPlayer_->setSource(url);
  }
  this->playMedia();
}

void Player::changeFullScreenStatus() {
  if (this->isFullScreen_) {
    this->ui_->groupPlay->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->ui_->groupPlay->showFullScreen();
    this->ui_->controlPad->setHidden(true);
  } else {
    this->ui_->groupPlay->setWindowFlags(Qt::SubWindow);
    this->ui_->groupPlay->showNormal();
    this->ui_->controlPad->setHidden(false);
  }
}

void Player::closeFrameShow() {
  if (this->showFrame_) {
    this->frame_->setHidden(true);
    this->showFrame_ = false;
  }
}

void Player::addMediaItemSpace() {
  this->ui_->scrollMediaListLayout->addSpacerItem(
      new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void Player::deleteMediaItemBox(QWidget* widget) {
  widget->setParent(Q_NULLPTR);
  this->ui_->scrollMediaListLayout->removeWidget(widget);
}

void Player::addMediaItemBox(QWidget* widget) {
  this->ui_->scrollMediaListLayout->insertWidget(
      this->ui_->scrollMediaListLayout->count() - 1, widget);
}

void Player::addFloatTable(QPushButton* info, const QString& str,
                           const Position& position) {
  auto widget = new MetaDataFloatTable(nullptr);
  widget->setText(str);
  switch (position) {
    case Position::UP: {
      widget->setPosition(
          info->mapToGlobal(QPoint(0, 0)).x() - widget->width() / 2 +
              info->width() / 2,
          info->mapToGlobal(QPoint(0, 0)).y() - widget->height());
      break;
    };
    case Position::RIGHT: {
      widget->setPosition(info->mapToGlobal(QPoint(0, 0)).x() + info->width(),
                          info->mapToGlobal(QPoint(0, 0)).y() -
                              widget->height() / 2 + info->height() / 2);
      break;
    }
  }
  widget->setWindowFlags(Qt::Popup);
  widget->show();
}

auto Player::ui() const -> PlayerUiPtr { return this->ui_; }

auto Player::mediaPlayer() const -> QPointer<QMediaPlayer> {
  return this->mediaPlayer_;
}

auto Player::audioOutput() const -> QPointer<QAudioOutput> {
  return this->audioOutput_;
}

bool Player::rewind() const { return this->ui_->rewind->isChecked(); }

auto Player::duration() const -> qint64 {
  return this->mediaPlayer_->duration();
}

auto Player::totalTime() const -> qint64 { return this->duration() / 1000; }

auto Player::metaData() const -> QMediaMetaData {
  return this->mediaPlayer_->metaData();
}
auto Player::sliderVolume() const -> qreal {
  // returns a real number from 0 to 1.
  return static_cast<qreal>(this->ui_->volumeSlider->value()) /
         static_cast<qreal>(this->ui_->volumeSlider->maximum());
}
auto Player::sliderProgress() const -> qint64 {
  return this->ui_->progressSlider->value();
}

auto Player::comboBoxRate() const -> qreal {
  auto rate_map = QMap<QString, qreal>{
      {tr("0.25x"), 0.25}, {tr("0.5x"), 0.5}, {tr("1x"), 1.0},
      {tr("1.5x"), 1.5},   {tr("2x"), 2.0},
  };
  return rate_map.value(this->ui_->rate->currentText(), 1.0);
}

bool Player::endOfMedia() const {
  return this->mediaPlayer_->mediaStatus() == QMediaPlayer::EndOfMedia;
}

auto Player::url() const -> QUrl { return this->mediaPlayer_->source(); }

#pragma endregion

#pragma region slots

void Player::playMedia() {
  this->setButtonPlayIcon(false);
  this->mediaPlayer_->play();
}

[[maybe_unused]] void Player::stopMedia() {
  this->setButtonPlayIcon(true);
  this->mediaPlayer_->stop();
}

[[maybe_unused]] void Player::pauseMedia() {
  this->setButtonPlayIcon(true);
  this->mediaPlayer_->pause();
}

/**
 * @brief A slot to keep track of the progression of the media playback and
 * update the time label.
 * @xxx It seems that the `progress_slider` updates every 100 microseconds,
 * therefore there will be ~10 time label updates in one second, of which 90%
 * are useless.
 * @param progress: Current progression of the playback (in microsecond).
 */
void Player::progressing(const qint64& progress) {
  if (!this->ui_->progressSlider->isSliderDown()) {
    this->ui_->progressSlider->setValue(static_cast<int>(progress));
  }
  this->updateTimeLabel(progress / 1000);
}

/**
 * @brief A slot to handle the click of the `open` button. It opens a Qt file
 * dialog, if user opens a file, it will initialize that media immediately.
 * @todo The format availability of the file should be checked. If a unsupported
 * file is opened, it should be able to give an error message.
 */
void Player::onClickOpen() {
  qDebug() << "clicked: open";
  auto fileDialog = QFileDialog{this};
  fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
  fileDialog.setWindowTitle(tr("Open File"));
  if (fileDialog.exec() == QDialog::Accepted) {
    qDebug() << "open: accepted, " << fileDialog.selectedUrls();
    initMedia(fileDialog.selectedUrls()[0]);
  } else {
    qDebug() << "open: rejected";
  }
}

void Player::onClickFullScreen() {
  qDebug() << "clicked: fullScreen" << this->isFullScreen_;
  this->isFullScreen_ = !this->isFullScreen_;
  this->setIsFullScreenIcon();
  this->changeFullScreenStatus();
}

bool Player::eventFilter(QObject* obj, QEvent* e) {
  if (this->isFullScreen_ && obj == this->ui_->videoWidget) {
    if (e->type() == QEvent::HoverMove) {
      qDebug() << "mouse: move";
      emit this->showBar();
      this->ui_->controlPad->setHidden(false);
    }
  }
  if (obj == this->ui_->progressSlider) {
    if (e->type() == QEvent::ToolTip) {
      auto helpEvent = dynamic_cast<QHelpEvent*>(e);
      auto percent =
          helpEvent->pos().x() * 1.0 / this->ui_->progressSlider->width();
      emit this->showFrame(percent);
    } else if (e->type() != QEvent::Paint) {
      this->closeFrameShow();
    }
  }
  return QWidget::eventFilter(obj, e);
}

void Player::dragEnterEvent(QDragEnterEvent* event) {
  if (event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

void Player::dropEvent(QDropEvent* event) {
  auto urls = event->mimeData()->urls();
  if (!urls.isEmpty()) {
    for (const auto& u : urls) {
      qDebug() << "drop:" << u.toString();
      this->initMedia(u);
    }
  }
}

#pragma endregion

#pragma region private

/**
 * @brief Update time label.
 * @param time: Current time (in second).
 */
void Player::updateTimeLabel(const qint64& time) {
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

auto Player::state() const -> QMediaPlayer::PlaybackState {
  return mediaPlayer_->playbackState();
}

#pragma endregion
