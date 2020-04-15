﻿#include "LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent) 
    : FramelessWindow(parent)
{
    InitializeUi();
    InitializeConnect();
}

void LoginWindow::InitializeUi()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(LoginWindowWidth, LoginWindowHeight);

    accountLineEdit = new QLineEdit(this);
    accountLineEdit->setFixedSize(LoginWindowAccountLineEditWidth, LoginWindowAccountLineEditHeight);
    accountLineEdit->move(LoginWindowAccountLineEditX, LoginWindowAccountLineEditY);
    accountLineEdit->setPlaceholderText("面宠账号");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setFixedSize(LoginWindowPasswordLineEditWidth, LoginWindowPasswordLineEditHeight);
    passwordLineEdit->move(LoginWindowPasswordLineEditX, LoginWindowPasswordLineEditY);
    passwordLineEdit->setPlaceholderText("密码");
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton(this);
    loginButton->setFixedSize(LoginWindowLoginButtonWidth, LoginWindowLoginButtonHeight);
    loginButton->move(LoginWindowLoginButtonX, LoginWindowLoginButtonY);
    loginButton->setText("登录");

    closeButton = new QPushButton(this);
    closeButton->setFixedSize(LoginWindowCloseButtonWidth, LoginWindowCloseButtonHeight);
    closeButton->move(LoginWindowCloseButtonX, LoginWindowCloseButtonY);

    registerButton = new QPushButton(this);
    registerButton->setFixedSize(LoginWindowRegisterButtonWidth, LoginWindowRegisterButtonHeight);
    registerButton->move(LoginWindowRegisterButtonX, LoginWindowRegisterButtonY);
    registerButton->setText("注册账号");
}

void LoginWindow::InitializeConnect()
{
    connect(closeButton, &QPushButton::clicked, this, &LoginWindow::close);
}