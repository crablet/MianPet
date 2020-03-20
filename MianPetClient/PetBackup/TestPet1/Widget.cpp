#include "Widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), isDragging(false)
{
    auto *gif = new QMovie(R"(C:\Users\crabl\Desktop\TestPet\test.gif)");
    auto *label = new QLabel(this);
    label->move(0, 0);
    label->setMovie(gif);
    gif->start();

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(gif->currentImage().size());
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (isDragging)
        {
            const auto delta = event->globalPos() - startPos;
            this->move(framePos + delta);
        }
    }

    QWidget::mouseMoveEvent(event);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        startPos = event->globalPos();
        framePos = frameGeometry().topLeft();
    }

    QWidget::mousePressEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    isDragging = false;

    QWidget::mouseReleaseEvent(event);
}
