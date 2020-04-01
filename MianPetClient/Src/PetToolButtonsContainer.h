#ifndef PETTOOLBUTTONSCONTAINER_H
#define PETTOOLBUTTONSCONTAINER_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QIcon>

#include "Src/Config/UiConstants.h"

#include "FoodWindow.h"

class PetToolButtonsContainer : public QWidget
{
    Q_OBJECT
public:
    explicit PetToolButtonsContainer(QWidget *parent = nullptr);

private:
    QHBoxLayout *layout;
    QPushButton *foodButton, *cleanButton, *dummyButton1, *dummyButton2;
    FoodWindow *foodWindow;
};

#endif // PETTOOLBUTTONSCONTAINER_H
