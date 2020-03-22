#include "Widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), isDragging(false)
{
    auto *gif = new QMovie(R"(C:\Users\crabl\Desktop\TestPet\test.gif)", {}, this);
    auto *label = new QLabel(this);
    label->move(0, 0);
    label->setMovie(gif);
    gif->start();

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(gif->currentImage().size());

    petToolButtonsContainer = new PetToolButtonsContainer;
    petToolButtonsContainer->setFixedWidth(this->width());
    petToolButtonsContainer->setFixedHeight(50);
    petToolButtonsContainer->move(frameGeometry().topLeft() + QPoint(0, width() - 16));
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (isDragging)
        {
            const auto delta = event->globalPos() - startPos;
            this->move(framePos + delta);
            petToolButtonsContainer->move(framePos + delta + QPoint(0, width() - 16));
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

void Widget::enterEvent([[maybe_unused]] QEvent *event)
{
    petToolButtonsContainer->move(frameGeometry().topLeft() + QPoint(0, width() - 16));
    petToolButtonsContainer->show();
}

void Widget::leaveEvent([[maybe_unused]] QEvent *event)
{
    QTimer::singleShot(520, this, [=]()
    {
        if (!petToolButtonsContainer->underMouse())
        {
            petToolButtonsContainer->hide();
        }
    });
}
