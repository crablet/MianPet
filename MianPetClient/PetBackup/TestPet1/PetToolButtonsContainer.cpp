#include "PetToolButtonsContainer.h"

PetToolButtonsContainer::PetToolButtonsContainer(QWidget *parent) : QWidget(parent)
{
    layout = new QHBoxLayout(this);

    button1 = new QPushButton;
    button2 = new QPushButton;

    layout->addStretch();
    layout->addWidget(button1);
    layout->addStretch();
    layout->addWidget(button2);
    layout->addStretch();

    this->setLayout(layout);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
}
