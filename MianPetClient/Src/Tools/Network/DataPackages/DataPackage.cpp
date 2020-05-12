#include "DataPackage.h"

DataPackage::DataPackage()
{
    SetVersion(MianPetVersion);
    SetId(mianPetId);
}

void DataPackage::SetMethod(int method)
{
    json.insert("method", method);
}

int DataPackage::GetMethod() const
{
    return json["method"].toInt();
}

void DataPackage::SetHint(int hint)
{
    json.insert("hint", hint);
}

int DataPackage::GetHint() const
{
    return json["hint"].toInt();
}

void DataPackage::SetVersion(int version)
{
    json.insert("version", version);
}

int DataPackage::GetVersion() const
{
    return json["version"].toInt();
}

void DataPackage::SetId(const QString &id)
{
    json.insert("id", id);
}

QString DataPackage::GetId() const
{
    return json["id"].toString();
}

void DataPackage::SetPayload(const QJsonObject &payload)
{
    this->payload = payload;
}

QJsonObject DataPackage::GetPayload() const
{
    return this->payload;
}

DataPackage::operator QByteArray()
{
    json.insert("payload", payload);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}
