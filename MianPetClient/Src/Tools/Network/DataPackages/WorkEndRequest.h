#ifndef WORKENDREQUEST_H
#define WORKENDREQUEST_H

#include <QString>

#include "RequestData.h"

// 用于向服务器请求是否能停止打工的数据包
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": GET,
//  "hint": WORKEND,
//  "random_key": string,
//  "payload":
//  {
//     "job": string    // 当该项为空时意味着结束当前正在工作的项目
//  }
//}
class WorkEndRequest : public RequestData
{
public:
    WorkEndRequest();

public:
    void SetJob(const QString &job);
    QString Getjob() const;
};

#endif // WORKENDREQUEST_H
