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
    foodButton->setStyleSheet("QPushButton { border: none }");  // 让按钮无边框

    cleanButton = new QPushButton(PetToolButtonsContainerWaterButtonIcon, {});
    cleanButton->setIconSize(PetToolButtonsContainerButtonSize);
    cleanButton->setFlat(true);
    cleanButton->setStyleSheet("QPushButton { border: none }"); // 让按钮无边框

    petProfileButton = new QPushButton;

    workingButton = new QPushButton(PetToolButtonsContainerWorkingButtonIcon, {});
    workingButton->setIconSize(PetToolButtonsContainerButtonSize);
    workingButton->setFlat(true);
    workingButton->setStyleSheet("QPushButton { border: none }"); // 让按钮无边框

    layout->addStretch();
    layout->addWidget(foodButton);
    layout->addStretch();
    layout->addWidget(cleanButton);
    layout->addStretch();
    layout->addWidget(workingButton);
    layout->addStretch();
    layout->addWidget(petProfileButton);
    layout->addStretch();

    this->setLayout(layout);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);  // Qt::Tool属性使得其任务栏图标被隐藏
    setAttribute(Qt::WA_TranslucentBackground);
}

void PetToolButtonsContainer::InitializeConnect()
{
    connect(foodButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnFoodButtonClicked);
    connect(cleanButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnCleanButtonClicked);
    connect(workingButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnWorkingButtonClicked);
    connect(petProfileButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnPetProfileButtonClicked);
}

void PetToolButtonsContainer::OnFoodButtonClicked()
{
    // 因为FoodWindow有只要退出就自动delete自身的特性，所以可以重复new
    foodWindow = new FoodWindow;
    foodWindow->show();

    // 点击以后就取消和这个槽的链接，就不会出现多次点击出现多个窗口的情况
    disconnect(foodButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnFoodButtonClicked);
    
    // 当窗口关闭时可以恢复链接，以响应下一次的信号
    connect(foodWindow, &FoodWindow::destroyed, [=]()
    {
        connect(foodButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnFoodButtonClicked);
    });
}

void PetToolButtonsContainer::OnCleanButtonClicked()
{
    // 因为CleanWindow有只要退出就自动delete自身的特性，所以可以重复new
    cleanWindow = new CleanWindow;
    cleanWindow->show();

    // 点击以后就取消和这个槽的链接，就不会出现多次点击出现多个窗口的情况
    disconnect(cleanButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnCleanButtonClicked);
    
    // 当窗口关闭时可以恢复链接，以响应下一次的信号
    connect(cleanWindow, &CleanWindow::destroyed, [=]()
    {
        connect(cleanButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnCleanButtonClicked);
    });
}

void PetToolButtonsContainer::OnWorkingButtonClicked()
{
    // 因为WorkingWindow有只要退出就自动delete自身的特性，所以可以重复new
    workingWindow = new WorkingWindow;
    workingWindow->show();

    // 点击以后就取消和这个槽的链接，就不会出现多次点击出现多个窗口的情况
    disconnect(workingButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnWorkingButtonClicked);

    // 当窗口关闭时可以恢复链接，以响应下一次的信号
    connect(workingWindow, &WorkingWindow::destroyed, [=]()
    {
        connect(workingButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnWorkingButtonClicked);
    });
}

void PetToolButtonsContainer::OnPetProfileButtonClicked()
{
    // 因为WorkingWindow有只要退出就自动delete自身的特性，所以可以重复new
    petProfileWindow = new PetProfileWindow;
    petProfileWindow->show();
    
    // 点击以后就取消和这个槽的链接，就不会出现多次点击出现多个窗口的情况
    disconnect(petProfileButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnPetProfileButtonClicked);
    
    // 当窗口关闭时可以恢复链接，以响应下一次的信号
    connect(petProfileWindow, &PetProfileWindow::destroyed, [=]()
    {
        connect(petProfileButton, &QPushButton::clicked, this, &PetToolButtonsContainer::OnPetProfileButtonClicked);
    });
}
