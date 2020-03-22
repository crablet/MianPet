#include "SmallItemsContainerWindow.h"

SmallItemsContainerWindow::SmallItemsContainerWindow(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setFixedSize(440, 190);

    closeButton = new QPushButton(this);
    closeButton->setIcon(QIcon(R"(C:\Users\crabl\Documents\Tencent Files\1063221915\FileRecv\qq-pet\close.png)"));
    closeButton->setIconSize(QSize(20, 20));
    closeButton->move(410, 0);
    closeButton->setFlat(true);    
}

void SmallItemsContainerWindow::mouseMoveEvent(QMouseEvent *event)
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

void SmallItemsContainerWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        startPos = event->globalPos();
        framePos = frameGeometry().topLeft();
    }

    QWidget::mousePressEvent(event);
}

void SmallItemsContainerWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isDragging = false;

    QWidget::mouseReleaseEvent(event);
}
