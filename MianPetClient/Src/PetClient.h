#ifndef PETCLIENT_H
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
    void HeartbeatError();

private slots:
    void Heartbeat();

    void OnExitActionTriggered();
    void OnSettingsActionTriggered();

private:
    QMovie *petGif;         // 面宠主界面中正在播放的动画
    QLabel *petGifLabel;    // 面宠主界面中用于播放动画的控件
    QPoint previousMousePos, previousUiPos;

    LoginWindow *loginWindow;
    PetToolButtonsContainer *petToolButtonsContainer;
    SettingsWindow *settingsWindow;

    QTimer *heartbeat;

    QSystemTrayIcon *tray;
    QMenu *trayMenu;

    QAction *exitAction, *settingsAction;

    bool isUiBeingDragging;
};
#endif // PETCLIENT_H
