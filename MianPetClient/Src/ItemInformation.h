//
// Created by crablet on 2021/2/1.
//

#pragma once
#ifndef MIANPETCLIENT_ITEMINFORMATION_H
#define MIANPETCLIENT_ITEMINFORMATION_H

#include <QString>
#include <utility>

class ItemInformation
{
    ItemInformation
    (QString name, int price, int amount, QString description, int type, int food, int clean, int health)
            : name(std::move(name)), price(price), amount(amount), description(std::move(description)), type(type), food(food), clean(clean), health(health)
    {
    }

    QString name;
    int price;
    int amount;
    QString description;
    int type;
    int food, clean, health;
};


#endif //MIANPETCLIENT_ITEMINFORMATION_H
