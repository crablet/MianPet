#ifndef FOODWINDOW_H
#define FOODWINDOW_H

#include <QWidget>

#include "SmallItemsContainerWindow.h"

class FoodWindow final : public SmallItemsContainerWindow
{
    Q_OBJECT
public:
    explicit FoodWindow(QWidget *parent = nullptr);

private:
    void InitializeUi();
};

#endif // FOODWINDOW_H
