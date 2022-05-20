#include "MediaItemBox.h"
#include "ui_MediaItemBox.h"

MediaItemBox::MediaItemBox(Player *parent) :
    QGroupBox(parent),
    ui_(new Ui::MediaItemBox){
    ui_->setupUi(this);
    player_ = parent;

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

void MediaItemBox::onClickPlay(){
    player_->setMediaUrl(media_url_);
}

void MediaItemBox::onClickBtnInfo() {
    qDebug() << "clicked: BtnInfo";
    player_->addFloatTable(ui_->btnInfo, metadata_str_);
}

void MediaItemBox::setBtnPlay(bool play) {
    ui_->btnPlay->setIcon(QIcon(play ? ":/images/circle-play.svg" : ":/images/circle-pause.svg"));
}

MediaItemBox::~MediaItemBox(){
    delete ui_;
}
