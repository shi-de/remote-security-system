#ifndef __USART3_H
#define __USART3_H
#include "sys.h"
#include <string.h> // 必须包含，用于strstr等函数

// 你的控制变量，外部可调用
// 0: 代表 N (关/低电平)
// 1: 代表 Y (开/高电平)

void usart3_init(u32 bound);
void USART3_Parse_Command(char *cmd);

#endif