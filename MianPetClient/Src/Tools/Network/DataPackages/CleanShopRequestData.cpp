#include "CleanShopRequestData.h"

CleanShopRequestData::CleanShopRequestData()
{
    SetHint(FOOD_SHOP_INFO);
    SetItems({});
}

void CleanShopRequestData::SetItems(const QJsonArray &items)
{
    payload.insert("items", items);
}

QJsonArray CleanShopRequestData::GetItems() const
{
    return payload["items"].toArray();
}
