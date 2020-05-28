#ifndef PETTOOLBUTTONSCONTAINER_H
#define PETTOOLBUTTONSCONTAINER_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QIcon>

#include "Src/Config/UiConstants.h"

#include "FoodWindow.h"
#include "CleanWindow.h"
#include "PetProfileWindow.h"

class PetToolButtonsContainer : public QWidget
{
    Q_OBJECT
public:
    explicit PetToolButtonsContainer(QWidget *parent = nullptr);

private:
    void InitializeUi();
    void InitializeConnect();

private slots:
    void OnFoodButtonClicked();
    void OnCleanButtonClicked();
    void OnPetProfileButtonClicked();

private:
    QHBoxLayout *layout;
    QPushButton *foodButton, *cleanButton, *petProfileButton, *dummyButton2;
    FoodWindow *foodWindow;
    CleanWindow *cleanWindow;
    PetProfileWindow *petProfileWindow;
};

#endif // PETTOOLBUTTONSCONTAINER_H
