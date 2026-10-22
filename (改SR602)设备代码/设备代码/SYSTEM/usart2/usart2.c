#include "sys.h"
#include "usart2.h"	  
#include "usart1.h"
#include "ESP8266.h" 
 
void uart2_init(u32 bound){
	 //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//使能USART2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//使能GPIOA时钟
	USART_DeInit(USART2);  //复位串口2
	 //USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
 
	 //USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3
 
	//USART 初始化设置
 
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
 
	 USART_Init(USART2, &USART_InitStructure); //初始化串口
	#if EN_USART2_RX		  //如果使能了接收  
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
 
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
//	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);   // 使能空闲中断
	#endif
	USART_Cmd(USART2, ENABLE);                    //使能串口 
 
}
 
char UsartReceiveBuff2[200] = {0};

void USART2_IRQHandler(void)  
{
    char ReceiveTemp;
    static uint8_t ReceiveCount = 0;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        ReceiveTemp = USART_ReceiveData(USART2);
        if (ReceiveTemp == '\n') // 以'\n'为结束符
        {
					ESP8266_MQTT_GetMsgAndPara(UsartReceiveBuff2, ReceiveCount);
					memset(UsartReceiveBuff2, 0, sizeof(UsartReceiveBuff2));
					ReceiveCount = 0;
        }
        else
        {
            UsartReceiveBuff2[ReceiveCount++] = ReceiveTemp;
        }
		}
} 