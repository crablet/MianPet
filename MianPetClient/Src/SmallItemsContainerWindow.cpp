#include "SmallItemsContainerWindow.h"

SmallItemsContainerWindow::SmallItemsContainerWindow(QWidget *parent) 
    : QWidget(parent), isUiBeingDragging(false)
{
    InitializeUi();
}

void SmallItemsContainerWindow::InitializeUi()
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setFixedSize(SmallItemsContainerWindowWidth, SmallItemsContainerWindowHeight);

    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(QPixmap("./Pic/container-background.png")));
    setPalette(palette);
}

void SmallItemsContainerWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (isUiBeingDragging)
        {
            const auto delta = event->globalPos() - previousMousePos;
            this->move(previousUiPos + delta);
        }
    }

    QWidget::mouseMoveEvent(event);
}

void SmallItemsContainerWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isUiBeingDragging = true;
        previousMousePos = event->globalPos();
        previousUiPos = frameGeometry().topLeft();
    }

    QWidget::mousePressEvent(event);
}

void SmallItemsContainerWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isUiBeingDragging = false;

    QWidget::mouseReleaseEvent(event);
}


