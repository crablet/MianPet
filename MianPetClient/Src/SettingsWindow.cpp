#include "SettingsWindow.h"

SettingsWindow::SettingsWindow()
{
    InitializeUi();
    InitializeConnect();
}

void SettingsWindow::InitializeUi()
{
    setAttribute(Qt::WA_DeleteOnClose);

    closeButton = new QPushButton(this);
    closeButton->setFixedSize(SettingsWindowCloseButtonWidth, SettingsWindowCloseButtonHeight);
}

void SettingsWindow::InitializeConnect()
{
    connect(closeButton, &QPushButton::clicked, this, &SettingsWindow::close);
}
