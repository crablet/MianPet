#include "PetProfileWindow.h"

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
    setWindowFlags(windowFlags() | Qt::Tool);   // Qt::Tool属性使得其任务栏图标被隐藏
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);

    usernameLabel = new QLabel;
    idLabel = new QLabel;
    levelLabel = new QLabel;
    ageLabel = new QLabel;
    growthBar = new QProgressBar;
    foodBar = new QProgressBar;
    cleanBar = new QProgressBar;
    healthBar = new QProgressBar;
    moodBar = new QProgressBar;

    growthBar->setRange(0, 100);
    foodBar->setRange(0, 100);
    cleanBar->setRange(0, 100);
    healthBar->setRange(0, 100);
    moodBar->setRange(0, 100);

    levelLabel->setText(QString::number(levelValue));
    ageLabel->setText(QString::number(ageValue));
    growthBar->setValue(growthValue);
    foodBar->setValue(foodValue);
    cleanBar->setValue(cleanValue);
    healthBar->setValue(healthValue);
    moodBar->setValue(moodValue);

    // 这样设置可以让进度条的文字显示在中间，实测Ubuntu默认就是在中间但Windows需要手动设置
    // 所以就干脆统一都这样设置，以免造成平台间的差异
    growthBar->setAlignment(Qt::AlignCenter);
    foodBar->setAlignment(Qt::AlignCenter);
    cleanBar->setAlignment(Qt::AlignCenter);
    healthBar->setAlignment(Qt::AlignCenter);
    moodBar->setAlignment(Qt::AlignCenter);

    upperLayout = new QFormLayout(this);
    upperLayout->addRow("昵称：", usernameLabel);
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

    growthSpeedLabel = new QLabel("成长速度：" + QString::number(growthSpeedValue) + "/小时");
    statusLabel = new QLabel("状态：正在成长");
    onlineTimeLabel = new QLabel("在线时间：" + QString::number(onlineTimeValue) + "分钟");

    lowerLayout = new QVBoxLayout(this);
    lowerLayout->addWidget(growthSpeedLabel);
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

// 设置背景为圆角且由内而外是白到浅蓝
// 由于这里是继承QWidget，所以只能通过重写paintEvent的方式来自定义样式，而不可以由QSS来实现
// 参考文档：https://doc.qt.io/qt-5/stylesheet-reference.html
void PetProfileWindow::paintEvent(QPaintEvent *event)
{
    QRadialGradient radial(PetProfileWindowWidth / 2, PetProfileWindowHeight / 2,
                           PetProfileWindowHeight / 2, 
                           PetProfileWindowWidth / 2, PetProfileWindowHeight / 2);
    radial.setColorAt(0, Qt::white);
    radial.setColorAt(1, QColor(226, 246, 254));

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(radial);
    painter.setPen(Qt::transparent);
    painter.drawRoundedRect(rect(), 9, 9);  // 圆角矩形

    FramelessWindow::paintEvent(event);
}

void PetProfileWindow::UpdatePetProfile()
{
    auto tcpSocket = std::make_unique<QTcpSocket>();

    tcpSocket->connectToHost(ServerAddress, ServerPort, QTcpSocket::ReadWrite);
    if (!tcpSocket->waitForConnected())
    {
        return;
    }

    tcpSocket->write(PetProfileRequestData{});
    if (!tcpSocket->waitForBytesWritten())
    {
        return;
    }

    if (!tcpSocket->waitForReadyRead())
    {
        return;
    }
    
    const auto message = tcpSocket->readAll();
    emit CanUpdatePetProfile(message);
}

void PetProfileWindow::UpdatePetProfileHelper(const QByteArray &profile)
{
    if (profile.isEmpty())
    {
        QMessageBox::warning(this, "传输错误", "无法与服务器进行正常通信，请检查您的网络，若网络无问题请汇报该程序错误");

        return;
    }

    // 测试样例（中间故意不添加空格是为了模拟真实的发包环境）
    // {"nickname":"test name","id":"9999999","level":88,"age":1901,"growth":32,"food":32,"clean":32,"health":32,"mood":32,"grouth_speed":99,"status":"normal","online_time":101}

    const auto jsonDocument = QJsonDocument::fromJson(profile);
    if (jsonDocument.isObject())
    {
        // 缓存数据
        levelValue = jsonDocument["level"].toInt();
        ageValue = jsonDocument["age"].toInt();
        growthValue = jsonDocument["growth"].toInt();
        foodValue = jsonDocument["food"].toInt();
        cleanValue = jsonDocument["clean"].toInt();
        healthValue = jsonDocument["health"].toInt();
        moodValue = jsonDocument["mood"].toInt();
        growthSpeedValue = jsonDocument["growth_speed"].toInt();
        onlineTimeValue = jsonDocument["online_time"].toInt();

        // 使用缓存的数据
        usernameLabel->setText(jsonDocument["username"].toString());
        idLabel->setText(jsonDocument["id"].toString());
        levelLabel->setText(QString::number(levelValue));
        ageLabel->setText(QString::number(ageValue / 60) + "小时");
        growthBar->setValue(growthValue);    // TODO: 这几个要按照换算公式换回百分比
        foodBar->setValue(foodValue);        // TODO: 这几个要按照换算公式换回百分比
        cleanBar->setValue(cleanValue);      // TODO: 这几个要按照换算公式换回百分比
        healthBar->setValue(healthValue);    // TODO: 这几个要按照换算公式换回百分比
        moodBar->setValue(moodValue);        // TODO: 这几个要按照换算公式换回百分比
        growthSpeedLabel->setText("成长速度：" + QString::number(growthSpeedValue) + "/小时");
        statusLabel->setText("状态：" + QString::number(jsonDocument["status"].toInt()));
        onlineTimeLabel->setText("在线时间：" + QString::number(onlineTimeValue) + "分钟");
    }
    else
    {
        // 记录到错误日志中
    }
}
