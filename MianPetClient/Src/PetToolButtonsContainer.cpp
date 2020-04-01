#include "PetToolButtonsContainer.h"

PetToolButtonsContainer::PetToolButtonsContainer(QWidget *parent) : QWidget(parent)
{
    layout = new QHBoxLayout(this);

    foodButton = new QPushButton(PetToolButtonsContainerFoodButtonIcon, {});
    foodButton->setIconSize(PetToolButtonsContainerButtonSize);
    foodButton->setFlat(true);
    foodButton->setStyleSheet("QPushButton { border: none }");

    cleanButton = new QPushButton(PetToolButtonsContainerWaterButtonIcon, {});
    cleanButton->setIconSize(PetToolButtonsContainerButtonSize);
    cleanButton->setFlat(true);
    cleanButton->setStyleSheet("QPushButton { border: none }");

    dummyButton1 = new QPushButton;
    dummyButton2 = new QPushButton;

    layout->addStretch();
    layout->addWidget(foodButton);
    layout->addStretch();
    layout->addWidget(cleanButton);
    layout->addStretch();
    layout->addWidget(dummyButton1);
    layout->addStretch();
    layout->addWidget(dummyButton2);
    layout->addStretch();

    this->setLayout(layout);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // memory leaks, only for testing
    connect(foodButton, &QPushButton::clicked, [=]()
    {
        foodWindow = new FoodWindow;
        foodWindow->show();
    });
}
