#include "MediaList.h"

MediaList::MediaList(QPointer<Player> parent_)
{
    player_ = parent_;
    media_player_ = QPointer<QMediaPlayer>{new QMediaPlayer()};
    currIndex = -1;
    connect(media_player_, &QMediaPlayer::metaDataChanged, this, &MediaList::onChangeMetaData);
}

void MediaList::addMediaItemBox(QPointer<MediaItemBox> mediaItemBox){
    mediaList.append(mediaItemBox);
    player_->addMediaItemBox(mediaItemBox);
    connect(mediaItemBox, &MediaItemBox::play, this, &MediaList::checkCurrMedia);
    connect(mediaItemBox, &MediaItemBox::pause, this, &MediaList::pause);
    connect(mediaItemBox, &MediaItemBox::stop, this, &MediaList::stop);
    connect(mediaItemBox, &MediaItemBox::deleteMedia, this, &MediaList::deleteMedia);
}

void MediaList::onChangeMetaData(){
    qDebug() << "onChangeMetaData";
    QMediaMetaData metaData = media_player_->metaData();

    if(metaData.isEmpty()) return;

    MediaItemBox *widget = new MediaItemBox(player_);
    QString artist = metaData.value(QMediaMetaData::Author).toString();
    QString title = metaData.value(QMediaMetaData::Title).toString();
    widget->setMetaData(metaData);
    widget->setArtist(artist);
    widget->setTitle(title);
    widget->setMediaUrl(media_player_->source());

    if(!mediaList.length()) {
        currMediaItem = widget;
        currIndex = 0;
        currMediaItem->setActive(true);
    }
    addMediaItemBox(widget);
}

void MediaList::addMediaItem(QUrl url){
    // 检查是否重复
    for(int i = 0, len = mediaList.length(); i< len;i++){
        if(mediaList[i]->getMediaUrl() == url){
            return;
        }
    }
    media_player_->setSource(url);
}

void MediaList::playCurrMedia(){
   currMediaItem->setBtnPlay(true);
}

void MediaList::stopCurrMedia(){
   currMediaItem->setBtnPlay(false);
}

int MediaList::findMediaItem(QUrl url){
    for(int i = 0, len = mediaList.length(); i< len;i++){
        if(mediaList[i]->getMediaUrl() == url){
            return i;
        }
    }
    return -1;
}

void MediaList::checkCurrMedia(QUrl url){
    if(url == currMediaItem->getMediaUrl()) {
        qDebug()<<"play"<<url;
        emit play();
        return;
    }
    int index = findMediaItem(url);
    if(index>=0){
        qDebug()<<index;
        currMediaItem->setActive(false);
        currMediaItem = mediaList[index];
        currIndex = index;
        currMediaItem->setActive(true);
        emit changeCurrMedia(url);
    }
}

void MediaList::deleteMedia(QUrl url){
    int index = findMediaItem(url);
    if(index>=0){
        player_->deleteMediaItemBox(mediaList[index]);
        mediaList.removeAt(index);
        if(index == currIndex){
            onNextMedia();
        }
    }
}

void MediaList::onNextMedia(){
    switch(playOrder){
        case onlyOnce:{
            emit stop();
            break;
        };
        case inOrder:{
            currIndex = (currIndex + 1) % mediaList.length();
            currMediaItem->setActive(false);
            currMediaItem = mediaList[currIndex];
            currMediaItem->setActive(true);
            currMediaItem->setBtnPlay(false);
            emit changeCurrMedia(currMediaItem->getMediaUrl());
            break;
        };
        case randomLoop:{
            currIndex = (currIndex + int(rand())) % mediaList.length();
            currMediaItem->setActive(false);
            currMediaItem = mediaList[currIndex];
            currMediaItem->setActive(true);
            currMediaItem->setBtnPlay(false);
            emit changeCurrMedia(currMediaItem->getMediaUrl());
            break;
        };
        case singleLoop:{
            emit play();
            break;
        };
    }
    bool continuePlay = playOrder!= onlyOnce;
    player_->setButtonLabelPlay(!continuePlay);
}

void MediaList::onChangePlayOrder(){
  switch(playOrder){
    case onlyOnce:{
      qDebug() << "clicked: btn_play_order change to play onlyOnce";
      playOrder = inOrder;
      player_->setPlayOrderIcon(1);
      break;
    };
    case inOrder:{
      qDebug() << "clicked: btn_play_order change to play randomLoop";
      playOrder = randomLoop;
      player_->setPlayOrderIcon(2);
      break;
    };
    case randomLoop:{
      qDebug() << "clicked: btn_play_order change to play singleLoop";
      playOrder = singleLoop;
      player_->setPlayOrderIcon(3);
      break;
    };
    case singleLoop:{
      qDebug() << "clicked: btn_play_order change to play onlyOnce";
      playOrder = onlyOnce;
      player_->setPlayOrderIcon(0);
      break;
    };
    default:{
      qDebug() << "error!!";
    };
  }
}
