#ifndef LOGINREQUESTDATA_H
#define LOGINREQUESTDATA_H

#include <QString>
#include <QJsonObject>

#include "RequestData.h"

// 用于向服务器请求用于登录时的数据包，此包包含密码等敏感信息
// 其拥有以下格式：
//{
//  "version": int,
//  "id": string,
//  "method": GET,
//  "hint": LOGIN,
//  "random_key": string,
//  "payload":
//  {
//     "password": string
//  }
//}
class LoginRequestData : public RequestData
{
public:
    LoginRequestData();

public:
    void SetPassword(const QString &password);
    QString GetPassword() const;
};

#endif // LOGINREQUESTDATA_H
