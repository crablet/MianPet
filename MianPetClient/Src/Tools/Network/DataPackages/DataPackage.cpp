#include "DataPackage.h"

DataPackage::DataPackage()
{
    SetVersion(MianPetVersion);
}

void DataPackage::SetMethod(int method)
{
    json["method"] = method;
}

int DataPackage::GetMethod() const
{
    return json["method"].toInt();
}

void DataPackage::SetHint(int hint)
{
    json["hint"] = hint;
}

int DataPackage::GetHint() const
{
    return json["hint"].toInt();
}

void DataPackage::SetVersion(int version)
{
    json["version"] = version;
}

int DataPackage::GetVersion() const
{
    return json["version"].toInt();
}

void DataPackage::SetPayload(const QJsonObject &value)
{
    json["payload"] = value;
}

QJsonObject DataPackage::GetPayload() const
{
    return json["payload"].toObject();
}

DataPackage::operator const char*() const
{
    int dummy;
    jsonDoc = QJsonDocument(json);
    return jsonDoc.rawData(&dummy);
}
