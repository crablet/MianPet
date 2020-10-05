#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QPushButton>

#include "FramelessWindow.h"
#include "Src/Config/UiConstants.h"

class SettingsWindow final : public FramelessWindow
{
public:
    SettingsWindow();

private:
    void InitializeUi();
    void InitializeConnect();

private:
    QPushButton *closeButton;   // 界面右上角的关闭按钮
};

#endif // SETTINGSWINDOW_H
