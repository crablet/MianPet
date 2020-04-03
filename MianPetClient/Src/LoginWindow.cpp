#include "LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent) 
    : QWidget(parent), isUiBeingDragging(false)
{
    InitializeUi();
    InitializeConnect();
}

void LoginWindow::InitializeUi()
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
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

    loginButton = new QPushButton(this);
    loginButton->setFixedSize(LoginWindowLoginButtonWidth, LoginWindowLoginButtonHeight);
    loginButton->move(LoginWindowLoginButtonX, LoginWindowLoginButtonY);
    loginButton->setText("登录");

    closeButton = new QPushButton(this);
    closeButton->setFixedSize(LoginWindowCloseButtonWidth, LoginWindowCloseButtonHeight);
    closeButton->move(LoginWindowCloseButtonX, LoginWindowCloseButtonY);
}

void LoginWindow::InitializeConnect()
{
    connect(closeButton, &QPushButton::clicked, this, &LoginWindow::close);
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (isUiBeingDragging)
        {
            const auto delta = event->globalPos() - previousMousePos;
            this->move(previousUiPos + delta);
        }
    }

    QWidget::mouseMoveEvent(event);
}

void LoginWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isUiBeingDragging = true;
        previousMousePos = event->globalPos();
        previousUiPos = frameGeometry().topLeft();
    }

    QWidget::mousePressEvent(event);
}

void LoginWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isUiBeingDragging = false;

    QWidget::mouseReleaseEvent(event);
}
