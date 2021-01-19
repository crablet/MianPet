//
// Created by crablet on 2021/1/18.
//

#pragma once
#ifndef MIANPETSERVER_MIANPETDATACONSTANTS_H
#define MIANPETSERVER_MIANPETDATACONSTANTS_H

inline constexpr int FoodNormalOrAbnormal = 50;     // 饿和不饿的分界线
inline constexpr int FoodDeltaWhenNormal = 5;       // 不饿的时候每次心跳扣的饥饿值
inline constexpr int FoodDeltaWhenAbnormal = 10;    // 饿的时候每次心跳扣的饥饿值
inline constexpr int CleanNormalOrAbnormal = 50;    // 干净和不干净的分界线
inline constexpr int CleanDeltaWhenNormal = 5;      // 干净的时候每次心跳扣的清洁值
inline constexpr int CleanDeltaWhenAbnormal = 10;   // 不干净的时候每次心跳扣的清洁值

#endif //MIANPETSERVER_MIANPETDATACONSTANTS_H
