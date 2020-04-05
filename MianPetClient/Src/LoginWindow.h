#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

#include "Src/Config/UiConstants.h"

#include "FramelessWindow.h"

class LoginWindow : public FramelessWindow
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

private:
    void InitializeUi();
    void InitializeConnect();

private:
    QLineEdit *accountLineEdit, *passwordLineEdit;
    QPushButton *loginButton, *closeButton, *registerButton;
};

#endif // LOGINWINDOW_H
