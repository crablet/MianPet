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

    auto *exitAction = new QAction("退出", this);
    connect(exitAction, &QAction::triggered, [=]()
    {
        qApp->quit();
    });

    trayMenu = new QMenu(this);
    trayMenu->addAction(exitAction);

    tray = new QSystemTrayIcon(this);
    tray->setContextMenu(trayMenu);
    tray->setIcon(QIcon(R"(:/Pic/test.gif)"));
    tray->setToolTip("未登录");
    tray->show();

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

        // 登录成功后开始每分钟发送一次心跳包
        using namespace std::chrono_literals;
        heartbeat = new QTimer(this);
        connect(heartbeat, &QTimer::timeout, this, &PetClient::SendHeartbeat);
        heartbeat->start(1min);
    });

    connect(this, &PetClient::HeartbeatError, this, [this]()
    {
        QMessageBox::information(this, "心跳停止", "心跳包未能成功发送，请检查网络。");
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
    // 鼠标离开宠物主体时就开始计时，时间一到就隐藏底部工具栏
    QTimer::singleShot(PetToolButtonsContainerStayingTime, this, [=]()
    {
        if (!petToolButtonsContainer->underMouse())
        {
            petToolButtonsContainer->hide();
        }
    });
}

// 心跳包释放函数，每一分钟执行一次，一次发一个包，然后结束本次心跳的tcp链接
void PetClient::SendHeartbeat()
{
    std::thread heartbeatThread(
    [this]()
    {
        auto socket = std::make_unique<QTcpSocket>();
        socket->connectToHost(ServerAddress, ServerPort);
        if (!socket->waitForConnected())
        {
            emit HeartbeatError();

            return;
        }

        socket->write(R"({"type":"heartbeat"})");
        if (!socket->waitForBytesWritten())
        {
            emit HeartbeatError();

            return;
        }
    });
    heartbeatThread.detach();
}
