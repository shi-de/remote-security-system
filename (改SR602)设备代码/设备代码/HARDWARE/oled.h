#ifndef _OLED_H
#define _OLED_H
#include "sys.h"
#include "stdlib.h"

// 显示模式定义
#define TYPE8X16    1
#define TYPE16X16   2
#define TYPE6X8     3

// OLED参数定义
#define OLED_WIDTH  128
#define OLED_HEIGHT 64
#define OLED_PAGES  8   // 64/8=8页

// OLED缓存大小
#define OLED_BUFFER_SIZE (OLED_WIDTH * OLED_PAGES)

// OLED命令
#define OLED_CMD  0x00
#define OLED_DATA 0x40

// 函数声明
void OLED_Init(void);
void OLED_CLS(void);
void OLED_Fill(u8 bmp_data);
void OLED_Print(u8 x, u8 y, u8 *ch, u8 char_size, u8 ascii_size);
#endif

