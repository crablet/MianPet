#ifndef DATAPACKAGE_H
#define DATAPACKAGE_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QByteArray>

#ifdef Q_OS_WIN
    #pragma execution_character_set("utf-8")
#endif // Q_OS_WIN

#include "Src/Config/NetworkConstants.h"

class DataPackage
{
public:
    DataPackage();

public:
    void SetMethod(int method);
    int GetMethod() const;

    void SetHint(int hint);
    int GetHint() const;

    void SetVersion(int version);
    int GetVersion() const;

    void SetPayload(const QJsonObject &payload);
    QJsonObject GetPayload() const;

    operator QByteArray();

protected:
    QJsonObject json, payload;
};

#endif // DATAPACKAGE_H
