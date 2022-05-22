#ifndef MEDIAITEMBOX_H
#define MEDIAITEMBOX_H

#include <QGroupBox>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include "Player.h"

namespace Ui {
class MediaItemBox;
}

class MediaItemBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit MediaItemBox(Player *parent = nullptr);
    ~MediaItemBox();
    void setTitle(QString title);
    void setArtist(QString author);
    void setImage(QImage img);
    void setMetaData(QMediaMetaData data);
    void setMediaUrl(const QUrl &newMedia_url);
    QUrl getMediaUrl();
    void setBtnPlay(bool play);
    void setActive(bool active);

private:
    Ui::MediaItemBox *ui_;
    QMediaMetaData metadata_;
    QString metadata_str_;
    Player* player_;
    QUrl media_url_;
    bool isPlaying;

signals:
    play(QUrl url);
    pause();
    stop();
    deleteMedia(QUrl url);

private slots:
    void onClickPlay();
    void onClickBtnInfo();
    void onClickBtnDel();
};

#endif // MEDIALISTBOX_H
