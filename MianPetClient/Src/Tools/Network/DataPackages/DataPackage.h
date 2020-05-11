#ifndef DATAPACKAGE_H
#define DATAPACKAGE_H

#include <QString>
#include <QJsonDocument>
#include <QJsonValue>

#ifdef Q_OS_WIN
    #pragma execution_character_set("utf-8")
#endif // Q_OS_WIN

class DataPackage
{
public:
    DataPackage();

public:
    void SetMethod(int method);
    int GetMethod() const;

    void SetHint(int hint);
    int GetHind() const;

    void SetVersion(int version);
    int GetVersion() const;

    void SetPayload(const QJsonValue &value);
    QJsonValue GetPayload() const;

    operator const char*() const;

protected:
    QJsonDocument json;
};

#endif // DATAPACKAGE_H
