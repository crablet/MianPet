#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "FramelessWindow.h"

class SettingsWindow final : public FramelessWindow
{
public:
    SettingsWindow();

private:
    void InitializeUi();
    void InitializeConnect();
};

#endif // SETTINGSWINDOW_H
