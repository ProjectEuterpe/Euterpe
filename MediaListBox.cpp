#include "MediaListBox.h"
#include "ui_MediaListBox.h"

MediaListBox::MediaListBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::MediaListBox)
{
    ui->setupUi(this);
}

void MediaListBox::setPicture(QString picture){

}

void MediaListBox::setPos(float x, float y){

}

void MediaListBox::setTitle(QString title){
    if(title == "") title = "None";
    ui->textTitle->setText(title);
}

void MediaListBox::setAuthor(QString author){
    if(author == "") author = "None";
    ui->textAuthor->setText(author);
}

MediaListBox::~MediaListBox()
{
    delete ui;
}
