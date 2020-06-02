#ifndef HEARTBEATDATA_H
#define HEARTBEATDATA_H

#include "DataPackage.h"

// 用于向服务器发送心跳包时的数据包
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": HEARTBEAT,
//  "random_key": string
//  "payload":
//  {
//  }
//}
class HeartbeatData : public DataPackage
{
public:
    HeartbeatData();
};

#endif // HEARTBEATDATA_H
