#ifndef JOBSINFOREQUESTDATA_H
#define JOBSINFOREQUESTDATA_H

#include <QJsonArray>

#include "RequestData.h"

// 用于向服务器请求用于询问打工职位信息时的数据包
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": GET,
//  "hint": JOBS_INFO,
//  "random_key": string
//  "payload":
//  {
//     "jobs": array
//  }
//}
class JobsInfoRequestData : public RequestData
{
public:
    JobsInfoRequestData();

public:
    void SetJobs(const QJsonArray &jobs);
    QJsonArray GetJobs() const;
};

#endif // JOBSINFOREQUESTDATA_H
