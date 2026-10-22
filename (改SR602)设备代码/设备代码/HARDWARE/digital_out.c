#include "digital_out.h"

/**
 * @brief  数字输出初始化
 * @param  无
 * @retval 无
 */
void Digital_Out_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOA和GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    // 使能GPIOC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    // 关闭JTAG，使能SWD，才能使用PB3、PA15
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
    //A PA8 PA11 PA15
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 ;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    PAout(8) = 0;
//    PAout(11) = 0;

    //B PB3 PB4
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    PBout(3) = 1;
    PBout(4) = 1;

    //非采集类V1.0 板卡注释下面代码
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_Init(GPIOB, &GPIO_InitStructure);
    // PBout(0) = 0;
    // PBout(1) = 0;
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_Init(GPIOC, &GPIO_InitStructure);
    // PCout(15) = 0;
}

