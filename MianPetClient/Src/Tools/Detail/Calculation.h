//
// Created by crablet on 2021/2/16.
//

#ifndef MIANPETCLIENT_CALCULATION_H
#define MIANPETCLIENT_CALCULATION_H

#include "PetStatusData.h"

// 将foodValue转换成百分比，返回值属于[0, 100];
inline int FoodValueToPercent(int foodValue)
{
    if (foodValue > FoodValueMax)
    {
        // log this error
        return 100;
    }
    else if (foodValue < 0)
    {
        // log this error
        return 0;
    }
    else
    {
        return 100 * foodValue / FoodValueMax;
    }
}

inline int CleanValueTOPercent

// 返回升级到n + 1级所需的总经验值，用于在第n级时展示升级进度
inline constexpr int GrowthValueMax(int n)
{
    ++n;    // 计算n + 1级，后面的公式根据文档来（MianPetServer/Doc/Rules.md）
    return (n % 2 == 0) ? (1 + n / 2) * n / 2
                        : ((n + 1) / 2) * ((n + 1) / 2);
}

#endif //MIANPETCLIENT_CALCULATION_H
