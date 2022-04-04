/*
 * Copyright (C) 2022 Mikra Selene
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "player.h"

#include "ui_Player.h"

Player::Player(QWidget *parent) : QWidget(parent), ui_(new Ui::Player) {
  ui_->setupUi(this);

#define on_click_connection(sender, slot) \
  connect(ui_->sender, &QPushButton::clicked, this, &Player::slot)
  on_click_connection(open, OnClickOpen);
  on_click_connection(prev, OnClickPrev);
  on_click_connection(play, OnClickPlay);
  on_click_connection(next, OnClickNext);
  on_click_connection(stop, OnClickStop);
  on_click_connection(info, OnClickInfo);
#undef on_click_connection

#define on_change_slider_connection(sender, signal, slot) \
  connect(ui_->sender, &QSlider::signal, this, &Player::slot)
  on_change_slider_connection(progress_slider, sliderMoved, OnChangeProgress);
  on_change_slider_connection(progress_slider, sliderPressed, OnChangeProgress);
  on_change_slider_connection(volume_slider, valueChanged, OnChangeVolume);
#undef on_change_slider_connection

  connect(ui_->rewind, &QCheckBox::stateChanged, this, &Player::OnChangeRate);
  connect(ui_->rate, &QComboBox::currentTextChanged, this,
          &Player::OnChangeRate);

  audio_ = new QAudioOutput;
  audio_->setVolume(SliderVolume());
  player_ = new QMediaPlayer;
  player_->setVideoOutput(ui_->video_widget);
  player_->setAudioOutput(audio_);

  connect(player_, &QMediaPlayer::positionChanged, this, &Player::Progressing);
  connect(player_, &QMediaPlayer::mediaStatusChanged, this, &Player::AtEnd);

#define control_connection(signal, receiver, slot) \
  connect(this, &Player::signal, receiver, slot)
  control_connection(Play, player_, &QMediaPlayer::play);
  control_connection(Pause, player_, &QMediaPlayer::pause);
  control_connection(Stop, player_, &QMediaPlayer::stop);
  control_connection(Prev, player_, []() -> void {});
  control_connection(Next, player_, []() -> void {});
  control_connection(Info, player_, []() -> void {});
  control_connection(ChangeVolume, audio_, &QAudioOutput::setVolume);
  control_connection(ChangeRate, player_, &QMediaPlayer::setPlaybackRate);
  control_connection(ChangeProgress, player_, &QMediaPlayer::setPosition);
  control_connection(Stop, ui_->video_widget,
                     QOverload<>::of(&QVideoWidget::update));
#undef control_connection
}

Player::~Player() { delete ui_; }

void Player::Progressing(qint64 progress) {
  if (!ui_->progress_slider->isSliderDown()) {
    ui_->progress_slider->setValue(static_cast<int>(progress));
  }
  UpdateTimeLabel(progress / 1000);
}

void Player::UpdateTimeLabel(qint64 time) {
  QString time_label_text;
  auto t = static_cast<qint32>(time);
  auto d = static_cast<qint32>(TotalTime());
  if (t != 0 || d != 0) {
    QTime current((t / 3600) % 60, (t / 60) % 60, t % 60, 0);
    QTime total((d / 3600) % 60, (d / 60) % 60, d % 60, 0);
    QString format(d > 3600 ? "hh:mm:ss" : "mm:ss");
    time_label_text = current.toString(format) + " / " + total.toString(format);
  }
  ui_->time_label->setText(time_label_text);
}

void Player::OnClickOpen() {
  qDebug() << "clicked: open";
  QFileDialog file_dialog(this);
  file_dialog.setAcceptMode(QFileDialog::AcceptOpen);
  file_dialog.setWindowTitle(tr("Open File"));
  if (file_dialog.exec() == QDialog::Accepted) {
    qDebug() << "open: accepted, " << file_dialog.selectedUrls();
    player_->setSource(file_dialog.selectedUrls()[0]);
    // todo: media list
    InitMedia();
  } else {
    qDebug() << "open: rejected";
  }
}

void Player::OnClickPlay() {
  qDebug() << "clicked: play";
  bool playing = State() == QMediaPlayer::PlayingState;
  emit playing ? Pause() : Play();
  ui_->progress_slider->setMaximum(static_cast<int>(Duration()));
  ui_->play->setText(QString(tr(playing ? "PLAY" : "PAUSE")));
}

void Player::OnClickStop() {
  qDebug() << "clicked: stop";
  emit Stop();
  ui_->play->setText(QString(tr("PLAY")));
}

void Player::OnClickPrev() {
  qDebug() << "clicked: prev";
  emit Prev();
}

void Player::OnClickNext() {
  qDebug() << "clicked: next";
  emit Next();
}

void Player::OnClickInfo() {
  qDebug() << "clicked: info";
  qDebug() << "media metadata:";
  auto metadata = MetaData();
  for (auto k : metadata.keys()) {
    qDebug() << k << ":" << metadata[k];
  }
  emit Info();
}

void Player::OnChangeProgress() {
  qDebug() << "changed: progress";
  emit ChangeProgress(ui_->progress_slider->value());
}

void Player::OnChangeVolume() {
  qDebug() << "changed: volume, " << ui_->volume_slider->value();
  emit ChangeVolume(SliderVolume());
}

void Player::OnChangeRate() {
  qDebug() << "changed: rate";
  qreal factor = 1.0;
  if (ui_->rewind->isChecked()) {
    factor = -1.0;
    ui_->loop->setChecked(false);
    ui_->loop->setEnabled(false);
  } else {
    ui_->loop->setEnabled(true);
  }
  emit ChangeRate(factor * ComboBoxRate());
}

auto Player::Duration() const -> qint64 { return player_->duration(); }

auto Player::TotalTime() const -> qint64 { return Duration() / 1000; }

auto Player::MetaData() const -> QMediaMetaData { return player_->metaData(); }

auto Player::State() const -> QMediaPlayer::PlaybackState {
  return player_->playbackState();
}

auto Player::SliderVolume() const -> float {
  return static_cast<float>(ui_->volume_slider->value()) /
         static_cast<float>(ui_->volume_slider->maximum());
}

auto Player::ComboBoxRate() const -> qreal {
  QMap<QString, qreal> rate_map = {
      {"0.25x", 0.25}, {"0.5x", 0.5}, {"1x", 1.0}, {"1.5x", 1.5}, {"2x", 2.0},
  };
  return rate_map.value(ui_->rate->currentText(), 1.0);
}

void Player::AtEnd() {
  if (player_->mediaStatus() == QMediaPlayer::EndOfMedia) {
    qDebug() << "end of media";
    bool loop = ui_->loop->isChecked();
    emit loop ? Play() : Stop();
    ui_->play->setText(QString(tr(loop ? "PAUSE" : "PLAY")));
  }
}

void Player::InitMedia() {
  qDebug() << "init media";
  ui_->play->setEnabled(true);
  ui_->play->setText(QString(tr("PLAY")));
  ui_->stop->setEnabled(true);
  auto metadata = MetaData();
  auto artist = metadata.value(QMediaMetaData::AlbumArtist).toString();
  auto title = metadata.value(QMediaMetaData::Title).toString();
  setWindowTitle(QString("%1 - %2").arg(artist == "" ? "V/A" : artist,
                                        title == "" ? "Untitled" : title));
}
