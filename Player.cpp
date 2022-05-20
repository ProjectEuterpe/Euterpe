/**
 * @file player.cpp
 * @author Mikra Selene
 * @version 1.0
 * @date 2022.04.05
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

#include "ui_Player.h"
#include "FloatTable.h"
#include "MediaItemBox.h"

Player::Player(const QPointer<QWidget>& parent)
    : QWidget(parent), ui_(new Ui::Player) {
  ui_->setupUi(this);

  // initialize audio output and media player
  audio_output_ = QPointer<QAudioOutput>{new QAudioOutput};
  media_player_ = QPointer<QMediaPlayer>{new QMediaPlayer};
  audio_output_->setVolume(sliderVolume());  // necessary?
  media_player_->setAudioOutput(audio_output_);
  media_player_->setVideoOutput(ui_->video_widget);
  setPlayOrderIcon(0);

  connect(ui_->open, &QPushButton::clicked,  //
          this, &Player::onClickOpen);
  connect(media_player_, &QMediaPlayer::positionChanged,  //
          this, &Player::progressing);
  connect(media_player_, &QMediaPlayer::metaDataChanged,  //
          this, &Player::onChangeMetaData);
  //全屏部分
  connect(ui_->isfullScreen,&QPushButton::clicked,  //
          this, &Player::onClickFullScreen);
  isfullScreen=false;
  ui_->video_widget->installEventFilter(this);
  //获取鼠标悬停事件
  ui_->video_widget->setAttribute(Qt::WA_Hover,true);
  ui_->progress_slider->setAttribute(Qt::WA_Hover,true);
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
  if (lock) {
    setPlayOrderIcon(0); // set playOrderIcon to onlyOnce
//    ui_->loop->setChecked(false);
//    ui_->loop->setEnabled(false);
    ui_->btn_play_order->setEnabled(false);
  } else {
//    ui_->loop->setEnabled(true);
    ui_->btn_play_order->setEnabled(true);
  }
}

/**
 * @brief Set the maximum value of the progress slider.
 * @param max
 */
void Player::setProgressSliderMax(int max) {
  ui_->progress_slider->setMaximum(max);
}

/**
 * @brief Set the `play` button label to "PLAY" or "PAUSE", depending on the
 * parameter `play`.
 * @param play: `true` if the button label is to be set to "PLAY", `false` if it
 * is to be set to "PAUSE".
 */
void Player::setButtonLabelPlay(bool play) {
//  ui_->play->setText(QString{tr(play ? "PLAY" : "PAUSE")});
   ui_->play->setIcon(QIcon(play ? ":/images/circle-play.svg" : ":/images/circle-pause.svg"));
}

/**
 * @brief set the icon of btn_volume button.
 * @param volume: true:volume>0 , false:volume=0.
 */
void Player::setButtonVolume(bool volume) {
    ui_->btn_volume->setIcon(QIcon(volume ? ":/images/volume-open.svg" : ":/images/volume-close.svg"));
}

/**
 * @brief set the icon of btn_play_order button.
 * @param type(index): Play order type (onlyOnce inOrder, randomLoop, singleLoop).
 */
void Player::setPlayOrderIcon(int type){
  const QString playOrderIcon[4] = {":/images/play-onlyOnce.svg",":/images/play-inOrder.svg",":/images/play-randomLoop.svg",":/images/play-singleLoop.svg"};
  const QString playOrderTip[4] = {"onlyOnce","inOrder","randomLoop","singleLoop"};
  ui_->btn_play_order->setToolTip(playOrderTip[type]);
  ui_->btn_play_order->setIcon(QIcon(playOrderIcon[type]));
}

void Player::setIsFullScreenIcon()
{
const QString isFullIcon[2] = {":/images/screen-full.svg",":/images/screen-common.svg"};

ui_->isfullScreen->setIcon(QIcon( isFullIcon[int(isfullScreen)]));
}

void Player::changeFullScreen()
{
 if(isfullScreen)
 {
    ui_->group_play->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
   ui_->group_play->showFullScreen();
     ui_->control_pad->setHidden(true);
 }
 else {
     ui_->group_play->setWindowFlags(Qt::SubWindow);
        ui_->group_play->showNormal();
         ui_->control_pad->setHidden(false);
 }
}

void Player::addFloatTable(QPushButton* info, QString str){
  FloatTable *widget = new FloatTable(nullptr);
  widget->setCustomText(str);
  widget->setCustomPos(info->mapToGlobal(QPoint(0,0)).x() - widget->width()/2 + info->width()/2,
                       info->mapToGlobal(QPoint(0,0)).y() - widget->height());
  widget->setWindowFlags(Qt::Popup);
  widget->show();
}

void Player::addMediaItemBox(QMediaMetaData metaData){
  if(metaData.isEmpty()) return;
  MediaItemBox *widget = new MediaItemBox(this);
  QString artist = "artist";
  QString title = "title";
  QVariant artistVar = metaData.value(QMediaMetaData::AlbumArtist);
  QVariant titleVar = metaData.value(QMediaMetaData::Title);
  if(artistVar.isNull()){
      artist = "unknow artist";
  } else {
      artist = metaData.value(QMediaMetaData::AlbumArtist).toString();
  }
  if(titleVar.isNull()){
      title = "unknow title";
  } else {
      title = metaData.value(QMediaMetaData::Title).toString();
  }
  widget->setMetaData(metaData);
  widget->setMediaUrl(media_url_);
  widget->setArtist(artist);
  widget->setTitle(title);
  qDebug()<<"addMediaBox"<<media_url_;
  ui_->scrollMediaListLayout->insertWidget(ui_->scrollMediaListLayout->count()-1, widget);
}

void Player::addMediaItemSpacerV(){
  ui_->scrollMediaListLayout->addSpacerItem(new QSpacerItem(20,40,QSizePolicy::Minimum,QSizePolicy::Expanding));
}

#pragma endregion

#pragma region  // region: get values and status of the player

auto Player::ui() const -> PlayerUiPtr { return ui_; }

auto Player::mediaPlayer() const -> QPointer<QMediaPlayer> {
  return media_player_;
}

auto Player::audioOutput() const -> QPointer<QAudioOutput> {
  return audio_output_;
}

bool Player::rewind() const { return ui_->rewind->isChecked(); }

//bool Player::loop() const { return ui_->loop->isChecked(); }
bool Player::isFullScreen() const { return isfullScreen; }
auto Player::duration() const -> qint64 { return media_player_->duration(); }

auto Player::totalTime() const -> qint64 { return duration() / 1000; }

auto Player::metaData() const -> QMediaMetaData {
    qDebug()<<media_player_->metaData().isEmpty();
    return media_player_->metaData();
}

auto Player::state() const -> QMediaPlayer::PlaybackState {
  return media_player_->playbackState();
}

auto Player::sliderVolume() const -> float {
  // returns a real number from 0 to 1.
  return static_cast<float>(ui_->volume_slider->value()) /
         static_cast<float>(ui_->volume_slider->maximum());
}
auto Player::sliderProgress() const -> qint64 {
  return ui_->progress_slider->value();
}

auto Player::comboBoxRate() const -> qreal {
  // todo
  auto rate_map = QMap<QString, qreal>{
      {"0.25x", 0.25}, {"0.5x", 0.5}, {"1x", 1.0}, {"1.5x", 1.5}, {"2x", 2.0},
  };
  return rate_map.value(ui_->rate->currentText(), 1.0);
}

bool Player::endOfMedia() const {
  return media_player_->mediaStatus() == QMediaPlayer::EndOfMedia;
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
  if (!ui_->progress_slider->isSliderDown()) {
    ui_->progress_slider->setValue(static_cast<int>(progress));
  }
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
    initMedia(file_dialog.selectedUrls()[0]);
  } else {
    qDebug() << "open: rejected";
  }
}

void Player::onClickFullScreen()
{
isfullScreen=!isfullScreen;

qDebug()<<"click:FullScreen:"<<isfullScreen;
setIsFullScreenIcon();
changeFullScreen();

}

bool Player::eventFilter(QObject *obj, QEvent *e)
{ //不是全屏状态 或者 不是播放框

   if(isfullScreen&& obj==ui_->video_widget)
   {
       if (e->type() == QEvent::HoverMove)//鼠标移动
           {
               qDebug()<<"mouse:move";
              emit showBar();
              ui_->control_pad->setHidden(false);
           }
   }

   // 事件交给上层对话框进行处理
       return QWidget::eventFilter(obj,e);
}

void Player::onChangeMetaData(){
  // 检查 media_url_ 是否存在与列表中 ？ todo

  addMediaItemBox(media_player_->metaData());
}

#pragma endregion

#pragma region  // region: private

/**
 * @brief Initialize a media file.
 * @param url: The url of the media file.
 */
void Player::initMedia(const QUrl& url) {
  qDebug() << "init media";
  media_url_ = url;
  media_player_->setSource(url);
  // some buttons remain unavailable until a media file is loaded.
  ui_->play->setEnabled(true);
  ui_->stop->setEnabled(true);
  setButtonLabelPlay(true);
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
            ui_->time_label->setText(time_label_text);
  }

void Player::setMediaUrl(const QUrl &newMedia_url){
  qDebug()<<"setMediaUrl";
  if(newMedia_url == media_url_) return;
  media_url_ = newMedia_url;
  media_player_->setSource(media_url_);
}



#pragma endregion
