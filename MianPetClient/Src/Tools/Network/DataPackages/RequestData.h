#ifndef REQUESTDATA_H
#define REQUESTDATA_H

#include "DataPackage.h"

// 该类是所有用于请求数据的数据包的基类，一般不直接使用，而是通过继承使用
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": GET,
//  "random_key": string
//  "payload":
//  {
//  }
//}
class RequestData : public DataPackage
{
public:
    RequestData();
};

#endif // REQUESTDATA_H
