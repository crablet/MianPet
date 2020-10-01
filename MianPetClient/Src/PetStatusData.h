#ifndef PETSTATUSDATA_H
#define PETSTATUSDATA_H

#include <QString>

inline int levelValue;  // 面宠等级
inline int ageValue;    // 面宠年龄
inline int growthValue; // 面宠成长速度
inline int foodValue;   // 面宠饥饿值
inline int cleanValue;  // 面宠清洁值
inline int healthValue; // 面宠健康度
inline int moodValue;   // 面宠心情值
inline int grouthSpeedValue;    // 面宠成长速度（单位/每小时）
inline int onlineTimeValue;     // 面宠在线时间（单位分钟）

inline bool isWorking;      // 标记是否正在打工：true -> 正在打工；false -> 没在打工
inline QString workingJob;  // 若isWorking为true，则该项保存正在打的工种，否则该项无意义
inline int workingTime;     // 若isWorking为true，则该项保存已经打工的时长，否则该项无意义

#endif // PETSTATUSDATA_H
