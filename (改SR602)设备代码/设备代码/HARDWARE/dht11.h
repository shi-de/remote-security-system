#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"

// 根据你的需求配置端口
#define DHT11_PORT      GPIOC
#define DHT11_PIN       GPIO_Pin_15
#define DHT11_PORT_RCC  RCC_APB2Periph_GPIOC

// 方便操作IO口的宏 (假设sys.h中有位带操作，如果没有则使用下面的库函数版本)
// 如果你的sys.h没有PBout/PBin定义，请在.c文件中使用库函数操作
#define DHT11_DQ_OUT    PCout(15) // 数据端口 输出
#define DHT11_DQ_IN     PCin(15)  // 数据端口 输入

// 函数原型
u8 DHT11_Init(void); // 初始化
u8 DHT11_Read_Data(u8 *temp, u8 *humi); // 读取温湿度

#endif