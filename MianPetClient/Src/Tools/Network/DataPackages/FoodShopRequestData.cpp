#include "FoodShopRequestData.h"

FoodShopRequestData::FoodShopRequestData()
{
    SetHint(FOOD_SHOP_INFO);
    SetItems({});
}

void FoodShopRequestData::SetItems(const QJsonArray &items)
{
    payload.insert("items", items);
}

QJsonArray FoodShopRequestData::GetItems() const
{
    return payload["items"].toArray();
}
