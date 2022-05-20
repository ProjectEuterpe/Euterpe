#ifndef MEDIALISTBOX_H
#define MEDIALISTBOX_H

#include <QGroupBox>

namespace Ui {
class MediaListBox;
}

class MediaListBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit MediaListBox(QWidget *parent = nullptr);
    ~MediaListBox();
    void setPicture(QString picture);
    void setPos(float x, float y);
    void setTitle(QString title);
    void setAuthor(QString author);

private:
    Ui::MediaListBox *ui;
};

#endif // MEDIALISTBOX_H
