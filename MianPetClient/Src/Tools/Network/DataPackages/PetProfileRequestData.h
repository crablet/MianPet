#ifndef PETPROFILEREQUESTDATA_H
#define PETPROFILEREQUESTDATA_H

#include "RequestData.h"

// 该类是所有用于请求数据的数据包的基类，一般不直接使用，而是通过继承使用
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": GET,
//  "hint": PETPROFILE,
//  "random_key": string,
//  "payload":
//  {
//  }
//}
class PetProfileRequestData : public RequestData
{
public:
    PetProfileRequestData();
};

#endif // PETPROFILEREQUESTDATA_H
