#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "sys.h" 

// 调试开关
#define DEBUG_ENABLE      1

#if DEBUG_ENABLE
    #define DEBUG_PRINTF(format, ...)    printf(format"\r\n", ##__VA_ARGS__)
    #define DEBUG_INFO(format, ...)      printf("[INFO] "format"\r\n", ##__VA_ARGS__)
    #define DEBUG_ERROR(format, ...)     printf("[ERROR] "format"\r\n", ##__VA_ARGS__)
    #define DEBUG_WARN(format, ...)      printf("[WARN] "format"\r\n", ##__VA_ARGS__)
#else
    #define DEBUG_PRINTF(format, ...)    
    #define DEBUG_INFO(format, ...)      
    #define DEBUG_ERROR(format, ...)     
    #define DEBUG_WARN(format, ...)      
#endif

void uart1_init(u32 bound);			
			
#endif


