#ifndef WORKSTATUSREQUESTDATA_H
#define WORKSTATUSREQUESTDATA_H

#include "RequestData.h"

// 用于向服务器请求用于询问面宠打工状态信息时的数据包
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": GET,
//  "hint": WORK_STATUS,
//  "random_key": string
//  "payload":
//  {
//  }
//}
class WorkStatusRequestData : public RequestData
{
public:
    WorkStatusRequestData();
};

#endif // WORKSTATUSREQUESTDATA_H
