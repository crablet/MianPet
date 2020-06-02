#ifndef COREKEYREQUESTDATA_H
#define COREKEYREQUESTDATA_H

#include "RequestData.h"

// 用于向服务器请求用于加密的corekey时的数据包
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": GET,
//  "hint": CORE_KEY_FOR_PASSWORD_TRANSPORTATION,
//  "random_key": string,
//  "payload":
//  {
//  }
//}
class CorekeyRequestData : public RequestData
{
public:
    CorekeyRequestData();
};

#endif // COREKEYREQUESTDATA_H
