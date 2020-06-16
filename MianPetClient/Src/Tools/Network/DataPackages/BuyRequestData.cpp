#include "BuyRequestData.h"

BuyRequestData::BuyRequestData()
{
    SetHint(BUY);
}

void BuyRequestData::SetItem(const QString &item)
{
    payload.insert("item", item);
}

QString BuyRequestData::GetItem() const
{
    return payload["item"].toString();
}

void BuyRequestData::SetCount(int count)
{
    payload.insert("count", count);
}

int BuyRequestData::GetCount() const
{
    return payload["count"].toInt();
}
