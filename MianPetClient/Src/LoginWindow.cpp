#include "LoginWindow.h"

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
    connect(registerButton, &QPushButton::clicked, this, 
    [=]() 
    { 
        QMessageBox::information(this, "暂不支持注册", "内测进行中，暂不开放注册功能，请联系开发者获取内测账号");
    });
}

//void LoginWindow::LoginThreadFunction()
//{
//    auto tcpSocket = std::make_unique<QTcpSocket>();
//    tcpSocket->connectToHost(ServerAddress, ServerPort, QTcpSocket::ReadWrite);
//    if (!tcpSocket->waitForConnected())
//    {
//        emit ConnectToHostFailed();
//
//        return;
//    }
//}
