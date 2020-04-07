#include "PetProfileWindow.h"

PetProfileWindow::PetProfileWindow(QWidget *parent) 
    : FramelessWindow(parent)
{
    InitializeUi();
}

void PetProfileWindow::InitializeUi()
{
    setFixedSize(PetProfileWindowWidth, PetProfileWindowHeight);

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
}
