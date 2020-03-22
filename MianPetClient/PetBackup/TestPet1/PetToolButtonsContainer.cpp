#include "PetToolButtonsContainer.h"

PetToolButtonsContainer::PetToolButtonsContainer(QWidget *parent) : QWidget(parent)
{
    smallWindow = new SmallItemsContainerWindow;

    layout = new QHBoxLayout(this);

    button1 = new QPushButton(QIcon(R"(C:\Users\crabl\Documents\Tencent Files\1063221915\FileRecv\qq-pet\bow.png)"), {});
    button1->setIconSize({ 43, 43 });
    button1->setFlat(true);
    button2 = new QPushButton(QIcon(R"(C:\Users\crabl\Documents\Tencent Files\1063221915\FileRecv\qq-pet\clean.png)"), {});
    button2->setIconSize({ 43, 43 });
    button2->setFlat(true);
    button3 = new QPushButton;
    button4 = new QPushButton;

    layout->addStretch();
    layout->addWidget(button1);
    layout->addStretch();
    layout->addWidget(button2);
    layout->addStretch();
    layout->addWidget(button3);
    layout->addStretch();
    layout->addWidget(button4);
    layout->addStretch();

    this->setLayout(layout);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(button1, &QPushButton::clicked, smallWindow, &SmallItemsContainerWindow::show);
}
