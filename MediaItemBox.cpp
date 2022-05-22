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
    connect(ui_->btnDel, &QPushButton::clicked, this, &MediaItemBox::onClickBtnDel);
}

MediaItemBox::~MediaItemBox(){
    delete ui_;
}


void MediaItemBox::setTitle(QString title){
    if(title == "") title = "Unknow Title";
    ui_->textTitle->setText(title);
    ui_->textTitle->setToolTip(title);
}

void MediaItemBox::setArtist(QString artist){
    if(artist == "") artist = "Unknow Artist";
    ui_->textArtist->setText(artist);
    ui_->textArtist->setToolTip(artist);
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

void MediaItemBox::setBtnPlay(bool play) {
    qDebug()<<"setBtnPlay"<<play;
    ui_->btnPlay->setIcon(QIcon(play ? ":/images/circle-play.svg" : ":/images/circle-pause.svg"));
}

void MediaItemBox::setActive(bool active){
    QString color = active ? "background-color: #f8f8f8" : "";
    this->setStyleSheet(color);
    if(!active) {
        isPlaying = false;
        setBtnPlay(!isPlaying);
    } else {
        emit pause();
    }
}

void MediaItemBox::onClickPlay(){
    qDebug()<<"onClickPlay"<<isPlaying;
    setBtnPlay(isPlaying);
    if(isPlaying){
        emit pause();
    }else{
        emit play(media_url_);
    }
    isPlaying = !isPlaying;
}

void MediaItemBox::onClickBtnInfo() {
    qDebug() << "clicked: BtnInfo";
    player_->addFloatTable(ui_->btnInfo, metadata_str_, 1);
}

void MediaItemBox::onClickBtnDel(){
    qDebug() << "clicked: BtnDel";
    emit deleteMedia(media_url_);
}
