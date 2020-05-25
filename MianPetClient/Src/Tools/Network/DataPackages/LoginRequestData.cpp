#include "LoginRequestData.h"

LoginRequestData::LoginRequestData()
{
    SetHint(LOGIN);
}

void LoginRequestData::SetPassword(const QString &password)
{
    payload.insert("password", password);
}

QString LoginRequestData::GetPassword() const
{
    return payload["password"].toString();
}
