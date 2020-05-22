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
//  "method": int,
//  "hint": int,    // 若"method"为GET才会有此字段
//  "payload":
//  {
//  }
//}
class DataPackage
{
public:
    DataPackage();

public:
    // 设置"method"字段
    void SetMethod(int method);
    // 获取"method"字段的数据
    int GetMethod() const;

    // 设置"hint"字段
    void SetHint(int hint);
    // 获取"hint"字段的数据
    int GetHint() const;

    // 设置"version"字段
    void SetVersion(int version);
    // 获取"version"字段的数据
    int GetVersion() const;

    // 设置"id"字段
    void SetId(const QString &id);
    // 获取"id"字段的数据
    QString GetId() const;

    // 设置"payload"字段
    void SetPayload(const QJsonObject &payload);
    // 获取"payload"字段的数据
    QJsonObject GetPayload() const;

    // 和QByteArray的类型转换运算符，适合用于QTcpSocket::write(const QByteArray&)函数中
    operator QByteArray();

protected:
    QJsonObject json, payload;
};

#endif // DATAPACKAGE_H
