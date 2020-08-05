#ifndef WORKBEGINREQUEST_H
#define WORKBEGINREQUEST_H

#include <QString>

#include "RequestData.h"

// 用于向服务器请求是否能开始打工的数据包
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": GET,
//  "hint": WORKBEGIN,
//  "random_key": string,
//  "payload":
//  {
//     "job": string
//  }
//}
class WorkBeginRequest : public RequestData
{
public:
    WorkBeginRequest();

public:
    void SetJob(const QString &job);
    QString Getjob() const;
};

#endif // WORKBEGINREQUEST_H
