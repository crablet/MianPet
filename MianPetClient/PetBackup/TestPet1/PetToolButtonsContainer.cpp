#include "PetToolButtonsContainer.h"

PetToolButtonsContainer::PetToolButtonsContainer(QWidget *parent) : QWidget(parent)
{
    layout = new QHBoxLayout(this);

    button1 = new QPushButton;
    button2 = new QPushButton;
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
}
