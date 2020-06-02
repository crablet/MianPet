#ifndef LOGOUTDATA_H
#define LOGOUTDATA_H

#include <QString>

#include "DataPackage.h"

#include "Src/Config/NetworkConstants.h"

// 用于传递登出消息的数据包
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": HEARTBEAT,
//  "random_key": string
//  "payload":
//  {
//   "random_key": string
//  }
//}
class LogoutData : public DataPackage
{
public:
    LogoutData();

private:
    // 设置"random_key"字段的值
    void SetRandomKey(const QString &randomKey);
    // 获取"random_key"字段的值
    QString GetRandomKey() const;
};

#endif // LOGOUTDATA_H
