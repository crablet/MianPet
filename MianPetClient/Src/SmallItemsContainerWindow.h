﻿#ifndef SMALLITEMSCONTAINERWINDOW_H
#define SMALLITEMSCONTAINERWINDOW_H

#include <QWidget>
#include <QPoint>
#include <QPalette>
#include <QPixmap>
#include <QBrush>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QSize>
#include <QObject>
#include <QEvent>
#include <QPoint>

#include "Src/Config/UiConstants.h"

#include "FramelessWindow.h"
#include "TransparentItemLabel.h"

class SmallItemsContainerWindow : public FramelessWindow
{
    Q_OBJECT
public:
    explicit SmallItemsContainerWindow(QWidget *parent = nullptr);

private:
    void InitializeUi();
    void InitializeConnect();

    bool eventFilter(QObject *object, QEvent *event) override;

public:
    void SetIcon(const QPixmap &icon, const QSize &size = QSize(0, 0));
    void SetWindowTitle(const QString &text);

signals:
    void MouseHoversEnterOnItem(QObject *item);
    void MouseHoversLeaveOnItem(QObject *item);

private:
    QPushButton *closeButton;
    QLabel *iconLabel, *windowTitleLabel;

protected:
    QPushButton *item0, *item1, *item2, *item3;
    QPushButton *previousPageButton, *nextPageButton;
    TransparentItemLabel *itemLabel;

    int currentPage;
};

#endif // SMALLITEMSCONTAINERWINDOW_H
