﻿#ifndef USEREQUESTDATA_H
#define USEREQUESTDATA_H

#include <QString>

#include "RequestData.h"

// 用于向服务器请求用于购买商品（食品、日用品和药品）时的数据包
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": GET,
//  "hint": BUY,
//  "random_key": string,
//  "payload":
//  {
//     "item": string,
//     "count": int
//  }
//}
class UseRequestData : public RequestData
{
public:
    UseRequestData();

public:
    void SetItem(const QString &item);
    QString GetItem() const;

    void SetCount(int count);
    int GetCount() const;
};

#endif // USEREQUESTDATA_H
