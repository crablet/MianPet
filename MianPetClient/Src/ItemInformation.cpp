//
// Created by crablet on 2021/2/1.
//

#include "ItemInformation.h"

const QString &ItemInformation::GetName() const
{
    return name;
}

void ItemInformation::SetName(const QString &name)
{
    this->name = name;
}

int ItemInformation::GetPrice() const
{
    return price;
}

void ItemInformation::SetPrice(int price)
{
    this->price = price;
}

int ItemInformation::GetAmount() const
{
    return amount;
}

void ItemInformation::SetAmount(int amount)
{
    this->amount = amount;
}

const QString &ItemInformation::GetDescription() const
{
    return description;
}

void ItemInformation::SetDescription(const QString &description)
{
    this->description = description;
}

int ItemInformation::GetType() const
{
    return type;
}

void ItemInformation::SetType(int type)
{
    this->type = type;
}

int ItemInformation::GetFood() const
{
    return food;
}

void ItemInformation::SetFood(int food)
{
    this->food = food;
}

int ItemInformation::GetClean() const
{
    return clean;
}

void ItemInformation::SetClean(int clean)
{
    this->clean = clean;
}

int ItemInformation::GetHealth() const
{
    return health;
}

void ItemInformation::SetHealth(int health)
{
    this->health = health;
}
