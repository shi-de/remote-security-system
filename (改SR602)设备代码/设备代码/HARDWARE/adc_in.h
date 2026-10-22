#ifndef __ADC_IN_H
#define __ADC_IN_H

#include "sys.h"

/*
* ADC通道
* | ADC1 通道  | GPIO 引脚 | 功能说明             |
*| -----| ----| ------------------------------- |
*| IN0  | PA0 | 外部模拟输入 0                   |
*| IN1  | PA1 | 外部模拟输入 1                   |
*| IN2  | PA2 | 外部模拟输入 2                   |
*| IN3  | PA3 | 外部模拟输入 3                   |
*| IN4  | PA4 | 外部模拟输入 4                   |
*| IN5  | PA5 | 外部模拟输入 5                   |
*| IN6  | PA6 | 外部模拟输入 6                   |
*| IN7  | PA7 | 外部模拟输入 7                   |
*| IN8  | PB0 | 外部模拟输入 8                   |
*| IN9  | PB1 | 外部模拟输入 9                   |
*| IN10 | PC0 | 外部模拟输入 10                  |
*| IN11 | PC1 | 外部模拟输入 11                  |
*| IN12 | PC2 | 外部模拟输入 12                  |
*| IN13 | PC3 | 外部模拟输入 13                  |
*| IN14 | PC4 | 外部模拟输入 14                  |
*| IN15 | PC5 | 外部模拟输入 15                  |
*| IN16 | 内部 | 温度传感器（仅 ADC1）            |
*| IN17 | 内部 | 内部参考电压 VREFINT（仅 ADC1）  |
*/

// 函数声明
void ADC_GPIO_Init(void);      // ADC GPIO初始化
void ADC1_Init(void);          // ADC1初始化
u16 ADC_PA0_GetValue(void);    // 获取PA0通道的ADC值
u16 ADC_PA1_GetValue(void);    // 获取PA1通道的ADC值
u16 ADC_PA4_GetValue(void);    // 获取PA4通道的ADC值
u16 ADC_PA5_GetValue(void);    // 获取PA5通道的ADC值

#endif
