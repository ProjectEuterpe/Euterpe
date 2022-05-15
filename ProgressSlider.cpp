#include "ProgressSlider.h"
#include"QMouseEvent"
ProgressSlider::ProgressSlider(QWidget *parent):QSlider(parent)
{
this->setMouseTracking(true);
}

void ProgressSlider::mouseDoubleClickEvent(QMouseEvent *ev)
{
    //获取当前点击位置,得到的这个鼠标坐标是相对于当前QSlider的坐标
       int currentX = ev->pos().x();
       //获取当前点击的位置占整个Slider的百分比
       double per = currentX *1.0 /this->width();
       qDebug() << "progress-doubleClick pos:"<<per;
       //发送双击事件
      emit onDoubleClick(per);

}








