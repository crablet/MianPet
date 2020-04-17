#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <thread>
#include <memory>

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QTcpSocket>
#include <QJsonDocument>

#include "Src/Config/UiConstants.h"
#include "Src/Config/NetworkConstants.h"

#include "FramelessWindow.h"

class LoginWindow : public FramelessWindow
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

private:
    void InitializeUi();
    void InitializeConnect();
    void LoginThreadFunction();

signals:
    void ConnectToHostFailed();
    void LoginSucceeded();
    void AccountOrPasswordWrong();
    void UnknownLoginError();

private:
    QLineEdit *accountLineEdit, *passwordLineEdit;
    QPushButton *loginButton, *closeButton, *registerButton;
};

#endif // LOGINWINDOW_H
