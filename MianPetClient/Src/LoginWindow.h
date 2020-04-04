#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>
#include <QLineEdit>

#include "Src/Config/UiConstants.h"

#ifdef Q_OS_WIN
    #pragma execution_character_set("utf-8")
#endif // Q_OS_WIN

class LoginWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

private:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    void InitializeUi();
    void InitializeConnect();

private:
    QLineEdit *accountLineEdit, *passwordLineEdit;
    QPushButton *loginButton, *closeButton, *registerButton;
    QPoint previousMousePos, previousUiPos;

    bool isUiBeingDragging;
};

#endif // LOGINWINDOW_H
