#include "sys.h"
#include "usart1.h"
#include "string.h"
#if SYSTEM_SUPPORT_OS
#include "includes.h"
#endif

#if 1
#pragma import(__use_no_semihosting)
struct __FILE
{
	int handle;
};

FILE __stdout;

void _sys_exit(int x)
{
	x = x;
}

int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
		; 
	USART1->DR = (u8)ch;
	return ch;
}

#endif

char UsartReceiveBuff[200] = {0};

void uart1_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = bound;                               
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 
    USART_InitStruct.USART_Parity = USART_Parity_No;                             
    USART_InitStruct.USART_StopBits = USART_StopBits_1;                         
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;                     
    USART_Init(USART1, &USART_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART1_IRQHandler(void)
{
    char ReceiveTemp;
    static uint8_t ReceiveCount = 0;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        ReceiveTemp = USART_ReceiveData(USART1);
        if (ReceiveTemp == '\n') // 以'\n'为结束符
        {
            DEBUG_INFO("接收到完整数据: %s\r\n", UsartReceiveBuff);
            memset(UsartReceiveBuff, 0, sizeof(UsartReceiveBuff));
            ReceiveCount = 0;
        }
        else
        {
            if (ReceiveCount >= sizeof(UsartReceiveBuff) - 1)
            {
                DEBUG_ERROR("接收缓冲区溢出!\r\n");
                ReceiveCount = 0;
                return;
            }
            UsartReceiveBuff[ReceiveCount++] = ReceiveTemp;
        }
    }
}