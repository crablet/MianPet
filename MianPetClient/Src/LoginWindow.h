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
#include <QIntValidator>

#include "Src/Config/UiConstants.h"
#include "Src/Config/NetworkConstants.h"

#include "Tools/Network/NetworkTools.h"
#include "Tools/Network/DataPackages/CorekeyRequestData.h"
#include "Tools/Network/DataPackages/LoginRequestData.h"
#include "spdlog/spdlog.h"

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
    void LoginSucceeded(const QString &account);
    void AccountOrPasswordWrong();
    void CannotGetCorekeyFromServer();
    void NotJsonObject();
    void UnknownLoginError();

private:
    QLineEdit *accountLineEdit, *passwordLineEdit;
    QPushButton *loginButton, *closeButton, *registerButton;
};

#endif // LOGINWINDOW_H
