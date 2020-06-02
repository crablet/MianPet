#ifndef FOODSHOPREQUESTDATA_H
#define FOODSHOPREQUESTDATA_H

#include <QJsonArray>

#include "RequestData.h"

// 用于向服务器请求用于询问食品商店信息时的数据包
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": GET,
//  "hint": FOOD_SHOP_INFO,
//  "random_key": string,
//  "payload":
//  {
//     "items": array
//  }
//}
class FoodShopRequestData : public RequestData
{
public:
    FoodShopRequestData();

public:
    void SetItems(const QJsonArray &items);
    QJsonArray GetItems() const;
};

#endif // FOODSHOPREQUESTDATA_H
