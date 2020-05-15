#include "LogoutData.h"

LogoutData::LogoutData()
{
    SetMethod(LOGOUT);
    SetRandomKey(randomKey);
}

void LogoutData::SetRandomKey(const QString &randomKey)
{
    payload.insert("random_key", randomKey);
}

QString LogoutData::GetRandomKey() const
{
    return payload["random_key"].toString();
}
