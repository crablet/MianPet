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

// 所有数据包的基类
// 转为待传输的json文件后会拥有以下的通用格式：
//{
//  "version": int,
//  "id": string,
//  "payload":
//  {
//  }
//}
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

    void SetId(const QString &id);
    QString GetId() const;

    void SetPayload(const QJsonObject &payload);
    QJsonObject GetPayload() const;

    operator QByteArray();

protected:
    QJsonObject json, payload;
};

#endif // DATAPACKAGE_H
