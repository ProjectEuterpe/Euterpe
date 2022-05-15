#ifndef PROGRESSSLIDER_H
#define PROGRESSSLIDER_H
#include<QSlider>
//使进度条获取鼠标点击位置
class ProgressSlider:public QSlider
{  Q_OBJECT
protected:
    void mouseDoubleClickEvent(QMouseEvent *ev);
signals:
    void onMouseMove();
    void onDoubleClick(const double);

public:
    ProgressSlider(QWidget *parent = nullptr);
};

#endif // PROGRESSSLIDER_H
