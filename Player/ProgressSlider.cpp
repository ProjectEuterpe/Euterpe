#include "ProgressSlider.h"

#include "QMouseEvent"
ProgressSlider::ProgressSlider(QWidget *parent) : QSlider(parent) {
  this->setMouseTracking(true);
  this->setAttribute(Qt::WA_Hover, true);
  isVideo = true;
}

void ProgressSlider::setIsVideo(bool is) { isVideo = is; }

void ProgressSlider::mouseMoveEvent(QMouseEvent *ev) {
  if (ev->buttons() & (Qt::LeftButton | Qt::RightButton) || !isVideo) {
    QSlider::mouseMoveEvent(ev);
    return;
  }
  //获取当前点击位置,得到的这个鼠标坐标是相对于当前QSlider的坐标
  int currentX = ev->pos().x();
  //获取当前点击的位置占整个Slider的百分比
  double per_ = currentX * 1.0 / this->width();
  //发送事件
  emit onMouseMove(per_);
}
