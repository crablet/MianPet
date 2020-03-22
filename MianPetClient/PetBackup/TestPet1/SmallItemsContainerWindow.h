#ifndef SMALLITEMSCONTAINERWINDOW_H
#define SMALLITEMSCONTAINERWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPoint>
#include <QMouseEvent>

class SmallItemsContainerWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SmallItemsContainerWindow(QWidget *parent = nullptr);

    //SmallItemsContainerWindow(const SmallItemsContainerWindow &) = delete;
    //SmallItemsContainerWindow(SmallItemsContainerWindow &&) = delete;
    //SmallItemsContainerWindow& operator=(const SmallItemsContainerWindow &) = delete;
    //SmallItemsContainerWindow& operaror=(SmallItemsContainerWindow &&) = delete;

private:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QLabel *iconLabel, *titleTextLabel;
    QPushButton *closeButton, *previousPageButton, *nextPageButton;

    QPoint startPos, framePos;
    bool isDragging;
};

#endif // SMALLITEMSCONTAINERWINDOW_H
