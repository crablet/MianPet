#ifndef LOGINREQUESTDATA_H
#define LOGINREQUESTDATA_H

#include <QString>
#include <QJsonObject>

#include "RequestData.h"

class LoginRequestData : public RequestData
{
public:
    LoginRequestData();

public:
    void SetPassword(const QString &password);
    QString GetPassword() const;

    void SetRandomKey(const QString &randomKey);
    QString GetRandomKey() const;
};

#endif // LOGINREQUESTDATA_H
