﻿#ifndef PETSTATUSDATA_H
#define PETSTATUSDATA_H

#include <QString>

inline int levelValue;  // 面宠等级
inline int ageValue;    // 面宠年龄
inline int growthValue; // 面宠成长速度
inline int foodValue;   // 面宠饥饿值
inline int cleanValue;  // 面宠清洁值
inline int healthValue; // 面宠健康度
inline int moodValue;
inline int grouthSpeedValue;
inline int onlineTimeValue;

inline bool isWorking;      // 标记是否正在打工：true -> 正在打工；false -> 没在打工
inline QString workingJob;  // 若isWorking为true，则该项保存正在打的工种
inline int workingTime;     // 若isWorking为true，则该项保存已经打工的时长

#endif // PETSTATUSDATA_H
