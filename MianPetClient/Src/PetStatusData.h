#ifndef PETSTATUSDATA_H
#define PETSTATUSDATA_H

#include <QString>

inline constexpr int FoodValueMax = 3600;   // 饥饿值的上限
inline constexpr int CleanValueMax = 3600;  // 清洁值的上限
inline constexpr int HealthValueMax = 100;  // 健康值的上限
inline constexpr int MoodValueMax = 100;    // 心情值的上限

// 返回升级到n + 1级所需的总经验值，用于在第n级时展示升级进度
inline constexpr int GrowthValueMax(int n)
{
    ++n;    // 计算n + 1级，后面的公式根据文档来（MianPetServer/Doc/Rules.md）
    return (n % 2 == 0) ? (1 + n / 2) * n / 2
                        : ((n + 1) / 2) * ((n + 1) / 2);
}

inline int levelValue;  // 面宠等级
inline int ageValue;    // 面宠年龄
inline int tuotuoValue; // 面宠拥有的坨坨数
inline int growthValue; // 面宠成长值
inline int foodValue;   // 面宠饥饿值
inline int cleanValue;  // 面宠清洁值
inline int healthValue; // 面宠健康度
inline int moodValue;   // 面宠心情值
inline int growthSpeedValue;    // 面宠成长速度（单位/每小时）
inline int onlineTimeValue;     // 面宠在线时间（单位分钟）

inline bool isWorking;      // 标记是否正在打工：true -> 正在打工；false -> 没在打工
inline QString workingJob;  // 若isWorking为true，则该项保存正在打的工种，否则该项无意义
inline int workingTime;     // 若isWorking为true，则该项保存已经打工的时长，否则该项无意义

#endif // PETSTATUSDATA_H
