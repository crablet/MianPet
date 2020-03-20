#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMovie>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>

class Widget : public QWidget
{
    Q_OBJECT

private:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPoint startPos, framePos;
    bool isDragging;

public:
    Widget(QWidget *parent = nullptr);
};
#endif // WIDGET_H
