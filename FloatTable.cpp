#include "FloatPan.h"
#include "ui_FloatPan.h"

FloatTable::FloatTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatTable)
{
    ui->setupUi(this);
    QFont font;
    font.setPixelSize(12);
    ui->textEdit->setFont(font);
}

FloatTable::~FloatTable(){
    delete ui;
}

void FloatTable::setCustomPos(float x1, float y1){
    this->setGeometry( x1, y1, ui->textEdit->width(), ui->textEdit->height());
}

void FloatTable::setCustomText(QString text){
    if(text == "") text = "You haven't chose the media to play~";
    ui->textEdit->setText(text);
}
