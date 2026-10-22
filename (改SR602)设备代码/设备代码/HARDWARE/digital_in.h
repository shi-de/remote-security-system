#ifndef __DIGITAL_IN_H
#define __DIGITAL_IN_H

#include "sys.h"

/**
 * 直接使用sys.h中的宏定义
 * 这里只初始化相应的GPIO
 * 输入直接读取即可 PAin(x)
 */

// 函数声明
void Digital_In_Init(void);      // 数字输入初始化

#endif
