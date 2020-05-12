#ifndef FOODSHOPREQUESTDATA_H
#define FOODSHOPREQUESTDATA_H

#include <QJsonArray>

#include "RequestData.h"

class FoodShopRequestData : public RequestData
{
public:
    FoodShopRequestData();

public:
    void SetItems(const QJsonArray &items);
    QJsonArray GetItems() const;
};

#endif // FOODSHOPREQUESTDATA_H
