#ifndef SMALLITEMSCONTAINERWINDOW_H
#define SMALLITEMSCONTAINERWINDOW_H

#ifdef Q_OS_WIN
    #pragma execution_character_set("utf-8")
#endif // Q_OS_WIN

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QPalette>
#include <QPixmap>
#include <QBrush>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QSize>

#include "Src/Config/UiConstants.h"

class SmallItemsContainerWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SmallItemsContainerWindow(QWidget *parent = nullptr);

private:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void InitializeUi();
    void InitializeConnect();

public:
    void SetIcon(const QPixmap &icon, const QSize &size = QSize(0, 0));
    void SetWindowTitle(const QString &text);

private:
    QPushButton *closeButton, *previousPageButton, *nextPageButton;
    QLabel *iconLabel, *windowTitleLabel;
    QPoint previousMousePos, previousUiPos;

    bool isUiBeingDragging;
};

#endif // SMALLITEMSCONTAINERWINDOW_H
