#include "SettingsWindow.h"

SettingsWindow::SettingsWindow()
{
    InitializeUi();
    InitializeConnect();
}

void SettingsWindow::InitializeUi()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(SettingsWindowWidth, SettingsWindowHeight);

    closeButton = new QPushButton(this);
    closeButton->setFixedSize(SettingsWindowCloseButtonWidth, SettingsWindowCloseButtonHeight);
    closeButton->move(SettingsWindowCloseButtonX, SettingsWindowCloseButtonY);
}

void SettingsWindow::InitializeConnect()
{
    connect(closeButton, &QPushButton::clicked, this, &SettingsWindow::close);
}
