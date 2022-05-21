#include "MediaItemBox.h"
#include "ui_MediaItemBox.h"

MediaItemBox::MediaItemBox(Player *parent) :
    QGroupBox(parent),
    ui_(new Ui::MediaItemBox){
    ui_->setupUi(this);
    player_ = parent;
    isPlaying = false;

    connect(ui_->btnPlay, &QPushButton::clicked, this, &MediaItemBox::onClickPlay);
    connect(ui_->btnInfo, &QPushButton::clicked, this, &MediaItemBox::onClickBtnInfo);
}

void MediaItemBox::setPicture(QString picture){

}

void MediaItemBox::setPos(float x, float y){

}

void MediaItemBox::setTitle(QString title){
    if(title == "") title = "None";
    ui_->textTitle->setText(title);
}

void MediaItemBox::setArtist(QString author){
    if(author == "") author = "None";
    ui_->textArtist->setText(author);
}

void MediaItemBox::setPicture(QImage img){
    ui_->picture->clear();
    ui_->picture->setPixmap(QPixmap::fromImage(img));
}

void MediaItemBox::setMetaData(QMediaMetaData data){
    metadata_ = data;
    metadata_str_ = "";
    for (auto k : metadata_.keys()) {
        metadata_str_ += metadata_.metaDataKeyToString(k) + ":" + metadata_[k].toString() + '\n';
    }
}

void MediaItemBox::setMediaUrl(const QUrl &newMedia_url){
    media_url_ = newMedia_url;
}
QUrl MediaItemBox::getMediaUrl(){
    return media_url_;
}

void MediaItemBox::setActive(bool active){
    QString color = active ? "background-color: #f8f8f8" : "";
    this->setStyleSheet(color);
    if(!active) {
        isPlaying = false;
        setBtnPlay(!isPlaying);
    }
}

void MediaItemBox::onClickPlay(){
    qDebug()<<"onClickPlay"<<isPlaying;
    setBtnPlay(isPlaying);
    if(isPlaying){
        player_->pauseMedia();
    }else{
        player_->setMediaUrl(media_url_);
    }
    isPlaying = !isPlaying;
}

void MediaItemBox::onClickBtnInfo() {
    qDebug() << "clicked: BtnInfo";
    player_->addFloatTable(ui_->btnInfo, metadata_str_);
}

void MediaItemBox::setBtnPlay(bool play) {
    qDebug()<<"setBtnPlay"<<play;
    ui_->btnPlay->setIcon(QIcon(play ? ":/images/circle-play.svg" : ":/images/circle-pause.svg"));
}

MediaItemBox::~MediaItemBox(){
    delete ui_;
}
