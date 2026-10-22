#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"


// 函数声明
void BEEP_Init(void);                          // 初始化
void BEEP_ON(void);                           // 开启
void BEEP_OFF(void);                          // 关闭
void BEEP_Toggle(void);                       // 切换状态

#endif
