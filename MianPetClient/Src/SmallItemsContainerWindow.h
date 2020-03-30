#ifndef SMALLITEMSCONTAINERWINDOW_H
#define SMALLITEMSCONTAINERWINDOW_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QPalette>
#include <QPixmap>
#include <QBrush>

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

private:
    QPoint previousMousePos, previousUiPos;

    bool isUiBeingDragging;
};

#endif // SMALLITEMSCONTAINERWINDOW_H
