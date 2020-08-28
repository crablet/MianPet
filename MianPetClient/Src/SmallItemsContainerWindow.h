#ifndef SMALLITEMSCONTAINERWINDOW_H
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
    // 设置窗口图标（不按比例拉伸填满）
    void SetIcon(const QPixmap &icon);
    // 设置窗口图标（按size拉伸）
    void SetIcon(const QPixmap &icon, const QSize &size);
    // 设置窗口标题
    void SetWindowTitle(const QString &text);
    // 设置窗口标题的宽度
    void SetWidthOfWindowTitle(int width);
    // 设置窗口标题的长度
    void SetHeightOfWindowTitle(int height);

signals:
    // 当鼠标放在中间四个格子之一的上面时，该信号被触发
    void MouseHoversMoveOnItem(QObject *item);
    // 当鼠标离开中间四个格子之一的上面时，该信号被触发
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
