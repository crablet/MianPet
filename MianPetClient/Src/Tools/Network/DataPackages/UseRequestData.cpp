#include "UseRequestData.h"

UseRequestData::UseRequestData()
{
    SetHint(USE);
}

void UseRequestData::SetItem(const QString &item)
{
    payload.insert("item", item);
}

QString UseRequestData::GetItem() const
{
    return payload["item"].toString();
}

void UseRequestData::SetCount(int count)
{
    payload.insert("count", count);
}

int UseRequestData::GetCount() const
{
    return payload["count"].toInt();
}