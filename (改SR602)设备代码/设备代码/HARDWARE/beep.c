#include "beep.h"
#include "delay.h"


/**
 * @brief  蜂鸣器初始化
 * @param  无
 * @retval 无
 */
void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    BEEP_OFF();  // 初始化为关闭状态
}

/**
 * @brief  蜂鸣器开启
 * @param  无
 * @retval 无
 */
void BEEP_ON(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

/**
 * @brief  蜂鸣器关闭
 * @param  无
 * @retval 无
 */
void BEEP_OFF(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

/**
 * @brief  蜂鸣器状态切换
 * @param  无
 * @retval 无
 */
void BEEP_Toggle(void)
{
    if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == 0)
        BEEP_ON();
    else
        BEEP_OFF();
}
