#include "DataPackage.h"

DataPackage::DataPackage()
{

}

void DataPackage::SetMethod(int method)
{
}

int DataPackage::GetMethod() const
{
    return 0;
}

void DataPackage::SetHint(int hint)
{
}

int DataPackage::GetHind() const
{
    return 0;
}

void DataPackage::SetVersion(int version)
{
}

int DataPackage::GetVersion() const
{
    return 0;
}

void DataPackage::SetPayload(const QJsonValue &value)
{
}

QJsonValue DataPackage::GetPayload() const
{
    return QJsonValue();
}

DataPackage::operator const char*() const
{
}
