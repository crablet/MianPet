#ifndef CLEANSHOPREQUESTDATA_H
#define CLEANSHOPREQUESTDATA_H

#include <QJsonArray>

#include "RequestData.h"

// 用于向服务器请求用于询问清洁品商店信息时的数据包
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": GET,
//  "hint": FOOD_SHOP_INFO,
//  "payload":
//  {
//     "items": array
//  }
//}
class CleanShopRequestData : public RequestData
{
public:
    CleanShopRequestData();

public:
    void SetItems(const QJsonArray &items);
    QJsonArray GetItems() const;
};

#endif // CLEANSHOPREQUESTDATA_H
