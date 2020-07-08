#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

#ifdef Q_OS_WIN
    #pragma execution_character_set("utf-8")
#endif // Q_OS_WIN

class FramelessWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FramelessWindow(QWidget *parent = nullptr);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPoint previousMousePos, previousUiPos;

    bool isUiBeingDragging;
};

#endif // FRAMELESSWINDOW_H
