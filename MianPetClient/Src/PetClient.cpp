#include "PetClient.h"

PetClient::PetClient(QWidget *parent)
    : QWidget(parent), isUiBeingDragging(false)
{
    InitializeUi();
    InitializeConnect();
}

void PetClient::InitializeUi()
{
    petGif = new QMovie(R"(:/Pic/test.gif)", {}, this);
    petGifLabel = new QLabel(this);
    petGifLabel->move(0, 0);
    petGifLabel->setMovie(petGif);
    petGif->start();

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(petGif->currentImage().size());
    setMouseTracking(true);

    petToolButtonsContainer = new PetToolButtonsContainer;
    petToolButtonsContainer->setFixedWidth(PetToolButtonsContainerWidth);
    petToolButtonsContainer->setFixedHeight(PetToolButtonsContainerHeight);
    petToolButtonsContainer->move(frameGeometry().topLeft() + QPoint(0, width() - PetToolButtonsContainerUiDelta));

    this->hide();
    loginWindow = new LoginWindow;
    loginWindow->show();
}

void PetClient::InitializeConnect()
{
    connect(loginWindow, &LoginWindow::LoginSucceeded, this, [=]()
    {
        this->show();
        loginWindow->close();
    });
}

void PetClient::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (isUiBeingDragging)
        {
            const auto delta = event->globalPos() - previousMousePos;
            this->move(previousUiPos + delta);
            petToolButtonsContainer->move(previousUiPos + delta + QPoint(0, width() - PetToolButtonsContainerUiDelta));
            // 这里要保证petToolButtonsContainer始终处于petClient的下方，同步运动
        }
    }

    QWidget::mouseMoveEvent(event);
}

void PetClient::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isUiBeingDragging = true;
        previousMousePos = event->globalPos();
        previousUiPos = frameGeometry().topLeft();
    }

    QWidget::mousePressEvent(event);
}

void PetClient::mouseReleaseEvent(QMouseEvent *event)
{
    isUiBeingDragging = false;

    QWidget::mouseReleaseEvent(event);
}

void PetClient::enterEvent([[maybe_unused]] QEvent *event)
{
    petToolButtonsContainer->move(frameGeometry().topLeft() + QPoint(0, width() - PetToolButtonsContainerUiDelta));
    petToolButtonsContainer->show();
}

void PetClient::leaveEvent([[maybe_unused]] QEvent *event)
{
    QTimer::singleShot(PetToolButtonsContainerStayingTime, this, [=]()
    {
        if (!petToolButtonsContainer->underMouse())
        {
            petToolButtonsContainer->hide();
        }
    });
}
