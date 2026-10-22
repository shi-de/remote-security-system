#include "uid_read.h"
#include <stdint.h>

UID_TypeDef UID_Read(void)
{
    UID_TypeDef uid;
    /* 绝对地址访问，不依赖库 */
    uid.w0 = *(uint32_t *)0x1FFFF7E8UL;
    uid.w1 = *(uint32_t *)0x1FFFF7ECUL;
    uid.w2 = *(uint32_t *)0x1FFFF7F0UL;
    return uid;
}
