#ifndef UID_READ_H
#define UID_READ_H

#include <stdint.h>

typedef struct {
    uint32_t w0;  /* 0x1FFFF7E8 */
    uint32_t w1;  /* 0x1FFFF7EC */
    uint32_t w2;  /* 0x1FFFF7F0 */
} UID_TypeDef;

/* 返回 96-bit UID 的 3 个 32-bit 字 */
UID_TypeDef UID_Read(void);

#endif