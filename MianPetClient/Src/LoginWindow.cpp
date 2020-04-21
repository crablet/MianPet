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
    accountLineEdit->setValidator(new QIntValidator(this)); //只允许账号输入框输入数字

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
        QMessageBox::information(this, "暂不支持注册", "内测进行中，暂不开放注册功能，请联系开发者获取内测账号。");
    });
    connect(loginButton, &QPushButton::clicked, this, 
    [=]()
    {
        if (accountLineEdit->text().isEmpty() || passwordLineEdit->text().isEmpty())
        {
            return;
        }
        else
        {
            loginButton->setDisabled(true); // 点击登录后就不应该再允许被点击，除非登陆失败

            std::thread loginThread(&LoginWindow::LoginThreadFunction, this);
            loginThread.detach();
        }
    });

    connect(this, &LoginWindow::ConnectToHostFailed, this, 
    [=]()
    {
        loginButton->setEnabled(true);

        QMessageBox::information(this, "连接失败", "无法连接至服务器，请检查网络。");
    });
    connect(this, &LoginWindow::AccountOrPasswordWrong, this,
    [=]()
    {
        loginButton->setEnabled(true);

        QMessageBox::information(this, "信息错误", "用户名或密码错误，请重新输入。");
    });
    connect(this, &LoginWindow::UnknownLoginError, this,
    [=]()
    {
        loginButton->setEnabled(true);

        QMessageBox::information(this, "未知错误", "出现未知错误，登陆失败。");
    });
}

void LoginWindow::LoginThreadFunction()
{
    auto tcpSocket = std::make_unique<QTcpSocket>();
    tcpSocket->connectToHost(ServerAddress, ServerPort, QTcpSocket::ReadWrite);
    if (!tcpSocket->waitForConnected())
    {
        emit ConnectToHostFailed();

        return;
    }

    const auto str 
        = (R"({"account":")" + accountLineEdit->text() 
         + R"(","password":")" + passwordLineEdit->text() 
         + R"(","random_key":")" + GetRandomKeyForPasswordTransportation(QString("123"))    /*这个coreKey是要从服务器上获取的，暂时先硬编码*/
         + R"("})")
         .toStdString();
    tcpSocket->write(str.c_str());
    if (!tcpSocket->waitForBytesWritten())
    {
        emit ConnectToHostFailed();

        return;
    }

    if (!tcpSocket->waitForReadyRead())
    {
        emit ConnectToHostFailed();

        return;
    }

    const auto message = tcpSocket->readAll();
    const auto jsonDocument = QJsonDocument::fromJson(message);
    if (jsonDocument.isObject())
    {
        const auto status = jsonDocument["status"].toString();
        if (status == "success")        // {"status":"success"}
        {
            emit LoginSucceeded();
        }
        else if (status == "failed")    // {"status":"failed"}
        {
            emit AccountOrPasswordWrong();
        }
        else                            // {"status":"hahaha"}
        {
            emit UnknownLoginError();
        }
    }
    else
    {
        emit UnknownLoginError();
    }
}
