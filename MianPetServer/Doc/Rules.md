﻿# 本页面主要记录对于服务器中数据的各种处理规则

## 经验等级计算公式

假设某账户的等级目前为n：  
则升级为n + 1时所需要的经验为：floor((n + 1) / 2)  
升级到n所需的总经验为：  
* (1 + n / 2) * n / 2 [当n为偶数];  
* ((n + 1) / 2) * ((n + 1) / 2) [当n为奇数]

## 成长速度计算公式
心情值显示为百分比，100%为满，0%为空。  
若心情值大于等于50%，则成长速度为2/小时；  
若心情值小于50%，则成长速度为1/小时。

## 各种数值的上限  
* 成长值：根据上方公式来计算
* 饥饿值：3600
* 清洁值：3600
* 健康值：100
* 心情值：100
