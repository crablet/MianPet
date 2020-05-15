#ifndef LOGOUTDATA_H
#define LOGOUTDATA_H

#include <QString>

#include "DataPackage.h"

#include "Src/Config/NetworkConstants.h"

class LogoutData : public DataPackage
{
public:
    LogoutData();

private:
    void SetRandomKey(const QString &randomKey);
    QString GetRandomKey() const;
};

#endif // LOGOUTDATA_H
