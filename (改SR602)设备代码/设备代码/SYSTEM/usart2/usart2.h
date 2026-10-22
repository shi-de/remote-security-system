#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "sys.h" 

#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
 
void uart2_init(u32 bound);
void Uart_Rece_Pares(void);
void USART2_SendChar(char ch);
void USART2_SendString(char* str);

#endif
 
 