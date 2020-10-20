#ifndef NETWORKTOOLS_H
#define NETWORKTOOLS_H

#include <random>
#include <string>

#include <QString>

// 生成形如“123456@coreKey@987654”的随机字符串
inline std::string GetRandomKeyForPasswordTransportation(const std::string &coreKey) noexcept
{
    std::random_device rd;
    std::minstd_rand gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);

    return std::to_string(dis(gen)) + '@' + coreKey + '@' + std::to_string(dis(gen));
}

// 生成形如“123456@coreKey@987654”的随机字符串
inline QString GetRandomKeyForPasswordTransportation(const QString &coreKey) noexcept
{
    return QString::fromStdString(GetRandomKeyForPasswordTransportation(coreKey.toStdString()));
}

#endif // NETWORKTOOLS_H
