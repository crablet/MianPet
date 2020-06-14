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
    connect(this, &LoginWindow::CannotGetCorekeyFromServer, this,
    [=]()
    {
        loginButton->setEnabled(true);

        QMessageBox::information(this, "登陆失败", "无法从服务器获取corekey，登陆失败。");
    });
    connect(this, &LoginWindow::NotJsonObject, this,
    [=]()
    {
        loginButton->setEnabled(true);

        QMessageBox::information(this, "解析错误", "收到的json并不是object格式，登陆失败。");
    });
    connect(this, &LoginWindow::UnknownLoginError, this,
    [=]()
    {
        loginButton->setEnabled(true);

        QMessageBox::information(this, "未知错误", "出现未知错误，登陆失败。");
    });
}

// 登录流程：
// 1. 连接服务器
// 2. 从服务器获取加密coreKey
// 3. 使用加密coreKey拼接待加密的明文
// 4. 加密使成为密文（暂未实现）
// 5. 发送密文至服务器
// 6. 接收从服务器传回的消息，此处是明文，直接读取并解析之
// 7. 解析结果后看是允许登录还是不允许
void LoginWindow::LoginThreadFunction()
{
    auto tcpSocket = std::make_unique<QTcpSocket>();
    tcpSocket->connectToHost(ServerAddress, ServerPort, QTcpSocket::ReadWrite);
    if (!tcpSocket->waitForConnected())
    {
        emit ConnectToHostFailed();

        return;
    }

    tcpSocket->write(CorekeyRequestData{});
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

    auto message = tcpSocket->readAll();
    auto jsonDocument = QJsonDocument::fromJson(message);
    QString coreKey;
    if (jsonDocument.isObject())
    {
        if (const auto status = jsonDocument["status"].toString(); 
            status.isEmpty())
        {
            coreKey = jsonDocument["corekey"].toString();   // {"corekey":"45678"}
            randomKey = GetRandomKeyForPasswordTransportation(coreKey);
        }
        else if (status == "failed")
        {
            emit CannotGetCorekeyFromServer();

            return;
        }
        else
        {
            emit UnknownLoginError();

            return;
        }
    }
    else
    {
        emit NotJsonObject();
    }

    tcpSocket = std::make_unique<QTcpSocket>(); // 这里每个tcp连接都是短连接，要想重新发数据必须重新新建一个tcp连接
    tcpSocket->connectToHost(ServerAddress, ServerPort, QTcpSocket::ReadWrite);
    if (!tcpSocket->waitForConnected())
    {
        emit ConnectToHostFailed();

        return;
    }

    LoginRequestData loginJson;
    loginJson.SetId(accountLineEdit->text());
    loginJson.SetPassword(passwordLineEdit->text());
    tcpSocket->write(loginJson);
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

    message = tcpSocket->readAll();
    jsonDocument = QJsonDocument::fromJson(message);
    if (jsonDocument.isObject())
    {
        const auto status = jsonDocument["status"].toString();
        if (status == "success")        // {"status":"success"}
        {
            emit LoginSucceeded(accountLineEdit->text());
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
        emit NotJsonObject();
    }
}
