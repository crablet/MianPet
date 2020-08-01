#include "SettingsWindow.h"

SettingsWindow::SettingsWindow()
{
    InitializeUi();
    InitializeConnect();
}

void SettingsWindow::InitializeUi()
{
    setAttribute(Qt::WA_DeleteOnClose);
}

void SettingsWindow::InitializeConnect()
{
}
