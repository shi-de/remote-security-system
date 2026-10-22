#ifndef __LED_H
#define __LED_H

#include "sys.h"

// LED引脚定义
#define LED_PIN     GPIO_Pin_14  // PC14

// LED状态定义
#define LED_ON    0   // LED亮(低电平点亮)
#define LED_OFF   1   // LED灭(高电平熄灭)

// 函数声明
void LED_Init(void);                  // LED初始化
void LED_Set(u8 state);           // 设置红色LED状态

#endif
