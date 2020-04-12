#include "PetProfileWindow.h"

#include <qdebug.h>

PetProfileWindow::PetProfileWindow(QWidget *parent) 
    : FramelessWindow(parent)
{
    InitializeUi();
    InitializeConnect();
}

void PetProfileWindow::InitializeUi()
{
    // 我们有理由相信：和服务器建立连接并从服务器获取有效数据所耗费的时间远比初始化UI和信号槽要长。
    // 所以为了缩短响应时间，我们就让获取数据的线程先执行，虽然这样有几率会出现数据比UI先获取并且信号槽还没建立的情况。
    // 但我们认为这种情况是很少见的，如果收到大量汇报说出现了此情况，则我们再调整这段代码的顺序。
    std::thread thread(&PetProfileWindow::UpdatePetProfile, this);
    thread.detach();

    setFixedSize(PetProfileWindowWidth, PetProfileWindowHeight);
    setAttribute(Qt::WA_DeleteOnClose);

    nicknameLabel = new QLabel;
    idLabel = new QLabel;
    levelLabel = new QLabel;
    ageLabel = new QLabel;
    growthBar = new QProgressBar;
    foodBar = new QProgressBar;
    cleanBar = new QProgressBar;
    healthBar = new QProgressBar;
    moodBar = new QProgressBar;

    upperLayout = new QFormLayout(this);
    upperLayout->addRow("昵称：", nicknameLabel);
    upperLayout->addRow("号码：", idLabel);
    upperLayout->addRow("等级：", levelLabel);
    upperLayout->addRow("年龄：", ageLabel);
    upperLayout->addRow("成长：", growthBar);
    upperLayout->addRow("饥饿：", foodBar);
    upperLayout->addRow("清洁：", cleanBar);
    upperLayout->addRow("健康：", healthBar);
    upperLayout->addRow("心情：", moodBar);
    upperLayout->setContentsMargins({ 2, 0, 2, 0 });

    upperWidget = new QWidget(this);
    upperWidget->setLayout(upperLayout);
    

    //////////////////////////////////////////////////////

    grouthSpeedLabel = new QLabel("成长速度：260/小时");
    statusLabel = new QLabel("状态：正在成长");
    onlineTimeLabel = new QLabel("在线时间：9分钟");

    lowerLayout = new QVBoxLayout(this);
    lowerLayout->addWidget(grouthSpeedLabel);
    lowerLayout->addWidget(statusLabel);
    lowerLayout->addWidget(onlineTimeLabel);
    lowerLayout->setContentsMargins({ 2, 0, 2, 0 });

    lowerWidget = new QWidget(this);
    lowerWidget->setLayout(lowerLayout);
    
    allLayout = new QVBoxLayout(this);
    allLayout->addWidget(upperWidget);
    allLayout->addWidget(lowerWidget);
    this->setLayout(allLayout);

    closeCountdownTimer = new QTimer(this);
}

void PetProfileWindow::InitializeConnect()
{
    connect(closeCountdownTimer, &QTimer::timeout, this, &PetProfileWindow::close);
    connect(this, &PetProfileWindow::CanUpdatePetProfile, this, &PetProfileWindow::UpdatePetProfileHelper);
}

// 鼠标进入显示范围则结束倒计时，否则鼠标多次重入时会出现问题
void PetProfileWindow::enterEvent([[maybe_unused]] QEvent *event)
{
    closeCountdownTimer->stop();
}

// 鼠标离开显示范围则开始倒计时，时间一到就自动关闭页面
void PetProfileWindow::leaveEvent([[maybe_unused]] QEvent *event)
{
    closeCountdownTimer->start(999);
}

void PetProfileWindow::UpdatePetProfile()
{
    auto tcpSocket = std::make_unique<QTcpSocket>();

    tcpSocket->connectToHost(ServerAddress, ServerPort, QTcpSocket::ReadWrite);
    if (!tcpSocket->waitForConnected())
    {
        return;
    }

    // 并不是要这样做，只是为了调试方便
    tcpSocket->write("get for petprofilewindow");
    if (!tcpSocket->waitForBytesWritten())
    {
        return;
    }

    if (!tcpSocket->waitForReadyRead())
    {
        return;
    }

    auto message = tcpSocket->readAll();
    emit CanUpdatePetProfile(message);
}

void PetProfileWindow::UpdatePetProfileHelper(const QString &profile)
{
    // 并不是要这样做，只是为了调试方便
    nicknameLabel->setText(profile);
}
