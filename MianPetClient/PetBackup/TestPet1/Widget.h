#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMovie>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QTimer>

#include "PetToolButtonsContainer.h"

class Widget : public QWidget
{
    Q_OBJECT

private:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:
    PetToolButtonsContainer *petToolButtonsContainer;
    QPoint startPos, framePos;
    bool isDragging;

public:
    Widget(QWidget *parent = nullptr);
};
#endif // WIDGET_H
