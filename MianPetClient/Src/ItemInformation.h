//
// Created by crablet on 2021/2/1.
//

#pragma once
#ifndef MIANPETCLIENT_ITEMINFORMATION_H
#define MIANPETCLIENT_ITEMINFORMATION_H

#include <QString>
#include <utility>

// 用于表示商店能买的一般商品的数据
class ItemInformation
{
public:
    ItemInformation() = default;
    ItemInformation
    (QString name, int price, int amount, QString description, int type, int food, int clean, int health)
            : name(std::move(name)), price(price), amount(amount), description(std::move(description)), type(type), food(food), clean(clean), health(health)
    {
    }

    [[nodiscard]] const QString &GetName() const;
    void SetName(const QString &name);

    [[nodiscard]] int GetPrice() const;
    void SetPrice(int price);

    [[nodiscard]] int GetAmount() const;
    void SetAmount(int amount);

    [[nodiscard]] const QString &GetDescription() const;
    void SetDescription(const QString &description);

    [[nodiscard]] int GetType() const;
    void SetType(int type);

    [[nodiscard]] int GetFood() const;
    void SetFood(int food);

    [[nodiscard]] int GetClean() const;
    void SetClean(int clean);

    [[nodiscard]] int GetHealth() const;
    void SetHealth(int health);


private:
    QString name;
    QString description;
    int price;
    int amount;
    int type;
    int food, clean, health;
};


#endif //MIANPETCLIENT_ITEMINFORMATION_H
