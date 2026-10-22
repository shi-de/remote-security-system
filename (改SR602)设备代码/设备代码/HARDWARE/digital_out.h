#ifndef __DIGITAL_OUT_H
#define __DIGITAL_OUT_H

#include "sys.h"

/**
 * 直接使用sys.h中的宏定义
 * 这里只初始化相应的GPIO
 * 输入直接读取即可 PAout(x)
 */

// 函数声明
void Digital_Out_Init(void);                    // 数字输出初始化

#endif
