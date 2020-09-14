#include "SettingsWindow.h"

SettingsWindow::SettingsWindow()
{
    InitializeUi();
    InitializeConnect();
}

void SettingsWindow::InitializeUi()
{
    setAttribute(Qt::WA_DeleteOnClose); // 当点击关闭时自动释放内存
    setFixedSize(SettingsWindowWidth, SettingsWindowHeight);

    closeButton = new QPushButton(this);
    closeButton->setFixedSize(SettingsWindowCloseButtonWidth, SettingsWindowCloseButtonHeight);
    closeButton->move(SettingsWindowCloseButtonX, SettingsWindowCloseButtonY);
}

void SettingsWindow::InitializeConnect()
{
    connect(closeButton, &QPushButton::clicked, this, &SettingsWindow::close);
}
