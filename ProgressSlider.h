#ifndef PROGRESSSLIDER_H
#define PROGRESSSLIDER_H
#include<QSlider>
#include<QPointer>
#include<QTimer>
//使进度条获取鼠标点击位置
class ProgressSlider:public QSlider
{  Q_OBJECT
    bool isVideo;
protected:
    void mouseMoveEvent(QMouseEvent *ev);
signals:
    void onMouseMove(const double);
public:
    ProgressSlider(QWidget *parent = nullptr);
    void setIsVideo(bool );
};

#endif // PROGRESSSLIDER_H
