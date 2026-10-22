#include "led.h"

/**
 * @brief  LED初始化
 * @param  无
 * @retval 无
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // 配置LED引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = LED_PIN;;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //关闭LED
    LED_Set(0);
}

/**
 * @brief  设置LED状态
 * @param  state: LED状态(LED_ON/LED_OFF)
 * @retval 无
 */
void LED_Set(u8 state)
{
    if(state == LED_ON)
        GPIO_ResetBits(GPIOC, LED_PIN);
    else
        GPIO_SetBits(GPIOC, LED_PIN);
}