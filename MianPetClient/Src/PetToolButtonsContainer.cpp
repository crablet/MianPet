#include "PetToolButtonsContainer.h"

PetToolButtonsContainer::PetToolButtonsContainer(QWidget *parent) : QWidget(parent)
{
    InitializeUi();
    InitializeConnect();
}

void PetToolButtonsContainer::InitializeUi()
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

    petProfileButton = new QPushButton;

    dummyButton2 = new QPushButton;

    layout->addStretch();
    layout->addWidget(foodButton);
    layout->addStretch();
    layout->addWidget(cleanButton);
    layout->addStretch();
    layout->addWidget(petProfileButton);
    layout->addStretch();
    layout->addWidget(dummyButton2);
    layout->addStretch();

    this->setLayout(layout);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void PetToolButtonsContainer::InitializeConnect()
{
    connect(foodButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnFoodButtonClicked);
    connect(cleanButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnCleanButtonClicked);
    connect(petProfileButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnPetProfileButtonClicked);
}

void PetToolButtonsContainer::OnFoodButtonClicked()
{
    foodWindow = new FoodWindow;
    foodWindow->show();
    disconnect(foodButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnFoodButtonClicked);
    connect(foodWindow, &FoodWindow::destroyed, [=]()
    {
        connect(foodButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnFoodButtonClicked);
    });
}

void PetToolButtonsContainer::OnCleanButtonClicked()
{
    cleanWindow = new CleanWindow;
    cleanWindow->show();
    disconnect(cleanButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnCleanButtonClicked);
    connect(cleanWindow, &CleanWindow::destroyed, [=]()
    {
        connect(cleanButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnCleanButtonClicked);
    });
}

void PetToolButtonsContainer::OnPetProfileButtonClicked()
{
    petProfileWindow = new PetProfileWindow;
    petProfileWindow->show();
    disconnect(petProfileButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnPetProfileButtonClicked);
    connect(petProfileWindow, &PetProfileWindow::destroyed, [=]()
    {
        connect(petProfileButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnPetProfileButtonClicked);
    });
}
