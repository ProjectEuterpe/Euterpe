/*
 * @file Player.h
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

#ifndef EUTERPE_PLAYER_PLAYER_H_
#define EUTERPE_PLAYER_PLAYER_H_

#include <QAudioOutput>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QMimeData>
#include <QPointer>
#include <QStackedWidget>
#include <QTime>
#include <QUrl>
#include <QVariant>
#include <QWidget>

#include "../MetaData/MetaDataFloatTable.h"

namespace Ui {
class Player;
}
using PlayerUiPtr = Ui::Player *;

enum class PlayOrder { OnlyOnce, InOrder, Random, SingleLoop };

enum class Position { UP, RIGHT };

#define nd [[nodiscard]]
#define mu [[maybe_unused]]

class Player : public QWidget {
  Q_OBJECT

 public:
  explicit Player(const QPointer<QWidget> &parent = Q_NULLPTR);
  ~Player() override;

 public:
  void initMedia(const QUrl &url);

  void setLoopLock(bool lock);
  void setProgressSliderMax(const qint32 &max);
  void setButtonPlayIcon(bool play);
  void setButtonPrevIcon();
  void setButtonNextIcon();
  void setButtonVolumeIcon(bool unmute);
  void setPlayOrderIcon(const PlayOrder &type);
  void setIsFullScreenIcon();
  void setFrame(const QImage &image);
  void setFramePos(const qreal &x);
  void setMediaUrl(const QUrl &url);
  void changeFullScreenStatus();
  void closeFrameShow();
  void deleteMediaItemBox(QWidget *widget);
  void addMediaItemSpace();
  void addMediaItemBox(QWidget *widget);
  static void addFloatTable(QPushButton *info, const QString &text,
                            const Position &position = Position::UP);

  nd auto ui() const -> PlayerUiPtr;
  nd auto mediaPlayer() const -> QPointer<QMediaPlayer>;
  nd auto audioOutput() const -> QPointer<QAudioOutput>;
  nd bool rewind() const;
  nd auto duration() const -> qint64;
  nd auto totalTime() const -> qint64;
  nd auto metaData() const -> QMediaMetaData;
  nd auto state() const -> QMediaPlayer::PlaybackState;
  nd auto sliderVolume() const -> qreal;
  nd auto sliderProgress() const -> qint64;
  nd auto comboBoxRate() const -> qreal;
  nd bool endOfMedia() const;
  nd auto url() const -> QUrl;

 private:
  void updateTimeLabel(const qint64 &time);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
 signals:
  void showBar();
  void addMedia(const QUrl &url);
  void showFrame(const qreal &percent);
#pragma clang diagnostic pop

 public slots:
  mu void playMedia();
  [[maybe_unused]] mu void stopMedia();
  mu void pauseMedia();

 private slots:
  void progressing(const qint64 &progress);
  void onClickOpen();
  void onClickFullScreen();
  bool eventFilter(QObject *, QEvent *) override;
  void dragEnterEvent(QDragEnterEvent *e) override;
  void dropEvent(QDropEvent *e) override;

 private:
  PlayerUiPtr ui_;
  bool isFullScreen_;
  bool showFrame_;
  QPointer<QMediaPlayer> mediaPlayer_;
  QPointer<QAudioOutput> audioOutput_;
  QPointer<MetaDataFloatTable> frame_;
  QUrl mediaUrl_;
};

#undef mu
#undef nd

#endif  // EUTERPE_PLAYER_PLAYER_H_
