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

void LoginRequestData::SetRandomKey(const QString &randomKey)
{
    payload.insert("random_key", randomKey);
}

QString LoginRequestData::GetRandomKey() const
{
    return payload["random_key"].toString();
}
