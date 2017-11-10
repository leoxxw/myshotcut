#include "basemainwidget.h"

BaseMainWidget::BaseMainWidget(QWidget *parent):
    QWidget(parent)
{

}

void BaseMainWidget::mousePressEvent(QMouseEvent *event)
{
    last = event->globalPos();
    QWidget::mousePressEvent(event);
}

void BaseMainWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    last = event->globalPos();
    move(x()+dx, y()+dy);
    QWidget::mouseMoveEvent(event);
}

void BaseMainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    QWidget::mouseReleaseEvent(event);
}
