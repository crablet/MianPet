#ifndef UITOOLS_H
#define UITOOLS_H

#include <QWidget>

#ifdef Q_OS_WIN
    #include <Windows.h>
    #pragma execution_character_set("utf-8")
#endif // Q_OS_WIN

// 隐藏widget在任务栏上的图标
inline void HideTaskbarIconOf(QWidget *widget) noexcept
{
#ifdef Q_OS_WIN
    auto hWnd = reinterpret_cast<HWND>(widget->winId());
    long style = GetWindowLong(hWnd, GWL_EXSTYLE);
    style = WS_EX_TOOLWINDOW;
    ShowWindow(hWnd, SW_HIDE);
    SetWindowLong(hWnd, GWL_EXSTYLE, style);
    ShowWindow(hWnd, SW_SHOW);
#endif // Q_OS_WIN

}

#endif // UITOOLS_H
