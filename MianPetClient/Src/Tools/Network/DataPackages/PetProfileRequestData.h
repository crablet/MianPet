#ifndef PETPROFILEREQUESTDATA_H
#define PETPROFILEREQUESTDATA_H

#include "RequestData.h"

// 用于向服务器请求更新面宠基础信息的数据包，请求后的数据将会用于PetProfile窗口中
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
