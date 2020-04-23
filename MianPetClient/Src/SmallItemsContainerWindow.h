#ifndef SMALLITEMSCONTAINERWINDOW_H
#define SMALLITEMSCONTAINERWINDOW_H

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
#include <QObject>
#include <QEvent>

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

private:
    QPushButton *closeButton, *previousPageButton, *nextPageButton;
    QLabel *iconLabel, *windowTitleLabel;

protected:
    QPushButton *item0, *item1, *item2, *item3;
    TransparentItemLabel *itemLabel;
};

#endif // SMALLITEMSCONTAINERWINDOW_H
