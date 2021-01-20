#include "PetClient.h"

PetClient::PetClient(QWidget *parent)
    : QWidget(parent), isUiBeingDragging(false)
{
    InitializeUi();
    InitializeConnect();
}

PetClient::~PetClient()
{
    Logout();   // 第二次登出，只是为了确保一定登出了，但还是有概率无法登出，需要服务器端强制执行登出，留给服务端的定时登出程序管理
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

    hidePetToolButtonsContainerTimer = new QTimer(this);
    hidePetToolButtonsContainerTimer->setSingleShot(true);
    hidePetToolButtonsContainerTimer->setInterval(PetToolButtonsContainerStayingTime);

    settingsAction = new QAction("设置", this);
    exitAction = new QAction("退出", this);

    trayMenu = new QMenu(this);
    trayMenu->addAction(settingsAction);
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
    // 登录成功的处理函数
    connect(loginWindow, &LoginWindow::LoginSucceeded, this, [=](const QString &id)
    {
        this->show();
        HideTaskbarIconOf(this);    // 这里使用Qt::Tool隐藏其任务栏图标会导致主界面闪退，因此才使用这个函数
        loginWindow->close();

        mianPetId = id;
        tray->setToolTip(mianPetId);    // 登录成功以后托盘提示要改成用户id（或用户名）

        // 登录成功后开始每分钟发送一次心跳包
        using namespace std::chrono_literals;
        heartbeat = new QTimer(this);
        connect(heartbeat, &QTimer::timeout, this, &PetClient::Heartbeat);
        heartbeat->start(1min);
    });

    connect(loginWindow, &LoginWindow::LoginWindowClosed, this,
    [=]()
    {
        tray->hide();
        delete tray;
        tray = nullptr;

        this->close();
    });

    // 处理收到错误的心跳包返回消息，应给出提示
    connect(this, &PetClient::HeartbeatError, this, [this]()
    {
        QMessageBox::information(this, "心跳停止", "心跳包未能成功发送，请检查网络。");
    });

    connect(exitAction, &QAction::triggered, this, &PetClient::OnExitActionTriggered);
    connect(settingsAction, &QAction::triggered, this, &PetClient::OnSettingsActionTriggered);

    // 鼠标离开宠物主体时就开始计时，时间一到就隐藏底部工具栏
    connect(hidePetToolButtonsContainerTimer, &QTimer::timeout, this, [=]()
    {
        if (!petToolButtonsContainer->underMouse())
        {
            petToolButtonsContainer->hide();
        }
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
    hidePetToolButtonsContainerTimer->stop();   // 先把之前的定时器停了，免得先前的定时器超时就把现在的隐藏了
    petToolButtonsContainer->move(frameGeometry().topLeft() + QPoint(0, width() - PetToolButtonsContainerUiDelta));
    petToolButtonsContainer->show();
}

void PetClient::leaveEvent([[maybe_unused]] QEvent *event)
{
    // 鼠标离开宠物主体时就开始计时，时间一到就隐藏底部工具栏
    hidePetToolButtonsContainerTimer->start();
}

void PetClient::Logout()
{
    if (!randomKey.isEmpty())   // 如果randomKey存在则说明登录流程至少已经走过一遍了，暂且不论走通没有
    {
        this->hide();   // 先隐藏主体让用户以为程序已经完全结束了，耗时的登出操作在后面慢慢做也可以，这样能流畅一些

        heartbeat->stop();  // 心跳包的发送要于登出数据的发送之前停止，以免出现退出之后服务器仍然收到心跳包导致重新上线的情况

        auto tcpSocket = std::make_unique<QTcpSocket>();
        tcpSocket->connectToHost(ServerAddress, ServerPort, QTcpSocket::ReadWrite);
        if (!tcpSocket->waitForConnected())
        {
            return;
        }

        tcpSocket->write(LogoutData{});
        tcpSocket->waitForBytesWritten();
    }
}

void PetClient::OnExitActionTriggered()
{
    Logout();

    qApp->quit();
}

void PetClient::OnSettingsActionTriggered()
{
    // 因为SettingsWindow有只要退出就自动delete自身的特性，所以可以重复new
    settingsWindow = new SettingsWindow;
    settingsWindow->show();

    // 点击以后就取消和这个槽的链接，就不会出现多次点击出现多个窗口的情况
    disconnect(settingsAction, &QAction::triggered, this, &PetClient::OnSettingsActionTriggered);

    // 当窗口关闭时可以恢复链接，以响应下一次的信号
    connect(settingsWindow, &SettingsWindow::destroyed, [=]()
    {
        connect(settingsAction, &QAction::triggered, this, &PetClient::OnSettingsActionTriggered);
    });
}

// 心跳包释放函数，每一分钟执行一次，一次发一个包，然后结束本次心跳的tcp链接
void PetClient::Heartbeat()
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

        socket->write(HeartbeatData{});
        if (!socket->waitForBytesWritten())
        {
            emit HeartbeatError();

            return;
        }

        // 发送完心跳包以后还需要接收的，但是目前服务器那边还没同步好，所以先没有这个功能
    });
    heartbeatThread.detach();
}
