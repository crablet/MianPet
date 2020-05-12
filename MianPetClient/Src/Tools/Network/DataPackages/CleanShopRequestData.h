#ifndef CLEANSHOPREQUESTDATA_H
#define CLEANSHOPREQUESTDATA_H

#include <QJsonArray>

#include "RequestData.h"

class CleanShopRequestData : public RequestData
{
public:
    CleanShopRequestData();

public:
    void SetItems(const QJsonArray &items);
    QJsonArray GetItems() const;
};

#endif // CLEANSHOPREQUESTDATA_H
