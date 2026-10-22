#ifndef __MODULE_TESTS_H
#define __MODULE_TESTS_H

#include "sys.h"

// 测试入口函数
void RunModuleTests(void);

// 测试模块开关宏定义
#define ENABLE_OLED_TEST    1  // OLED测试使能
#define ENABLE_KEY_TEST     0  // 按键测试使能
#define ENABLE_SENSOR_TEST  0  // 传感器测试使能

// 测试函数声明
#if ENABLE_OLED_TEST
void OLED_DisplayTest(void);    // OLED显示测试
void OLED_FillTest(void);       // OLED图形测试
void OLED_ChineseTest(void);    // OLED中文显示测试
#endif

#if ENABLE_KEY_TEST
void KEY_FunctionTest(void);  // 按键功能测试
#endif

#if ENABLE_SENSOR_TEST
void SENSOR_BasicTest(void);  // 传感器基本功能测试
#endif


#endif
