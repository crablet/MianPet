﻿#ifndef PETCLIENT_H
#define PETCLIENT_H

#include <chrono>
#include <thread>
#include <memory>

#include <QWidget>
#include <QMouseEvent>
#include <QMovie>
#include <QLabel>
#include <QTimer>
#include <QTcpSocket>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#include "LoginWindow.h"
#include "PetToolButtonsContainer.h"
#include "SettingsWindow.h"

#include "Src/Config/UiConstants.h"
#include "Src/Config/NetworkConstants.h"

#include "Tools/Network/DataPackages/HeartbeatData.h"
#include "Tools/Network/DataPackages/LogoutData.h"

#include "Tools/Ui/UiTools.h"

#ifdef Q_OS_WIN
    #pragma execution_character_set("utf-8")
#endif // Q_OS_WIN

// 面宠主窗口
class PetClient : public QWidget
{
    Q_OBJECT

public:
    PetClient(QWidget *parent = nullptr);
    ~PetClient();

private:
    void InitializeUi();
    void InitializeConnect();

    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    void Logout();

signals:
    void HeartbeatError();  // 当遇到与心跳包有关的错误时会发出此信号

private slots:
    void Heartbeat();

    void OnExitActionTriggered();
    void OnSettingsActionTriggered();

private:
    QMovie *petGif;         // 面宠主界面中正在播放的动画
    QLabel *petGifLabel;    // 面宠主界面中用于播放动画的控件
    QPoint previousMousePos, previousUiPos; // 用于实现无边框界面的辅助变量

    LoginWindow *loginWindow;       // 登录窗口
    PetToolButtonsContainer *petToolButtonsContainer;   // 主界面底部的按钮集合，有“打工”、“食品”、“清洁”等等
    SettingsWindow *settingsWindow; // 右键托盘图标能调出此设置页面

    QTimer *heartbeat;      // 负责发送心跳的定时器
    QTimer *hidePetToolButtonsContainerTimer;   // 用于隐藏主界面底部的按钮集合的定时器

    QSystemTrayIcon *tray;  // 系统托盘
    QMenu *trayMenu;        // 系统托盘的菜单

    QAction *exitAction;    // 托盘中退出程序的动作
    QAction *settingsAction;// 托盘中打开设置窗口的动作

    bool isUiBeingDragging; // 表示UI界面是否正在被拖动，用于实现无边框界面
};
#endif // PETCLIENT_H
