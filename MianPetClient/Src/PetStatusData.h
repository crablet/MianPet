#ifndef PETSTATUSDATA_H
#define PETSTATUSDATA_H

#include <QString>

inline int levelValue;  // 面宠等级
inline int ageValue;    // 面宠年龄
inline int growthValue;
inline int foodValue;
inline int cleanValue;
inline int healthValue;
inline int moodValue;
inline int grouthSpeedValue;
inline int onlineTimeValue;

inline bool isWorking;      // 标记是否正在打工：true -> 正在打工；false -> 没在打工
inline QString workingJob;  // 若isWorking为true，则该项保存正在打的工种
inline int workingTime;     // 若isWorking为true，则该项保存已经打工的时长

#endif // PETSTATUSDATA_H
