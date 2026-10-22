#include "tim_out.h"

static u16 g_arr = 0;  // 保存自动重装值，用于计算占空比

/**
 * @brief  定时器3 PWM初始化
 * @param  arr: 自动重装值
 * @param  psc: 预分频系数
 * @retval 无
 * @note   PWM频率 = 72M/((arr+1)*(psc+1))
 */
void TIM3_PWM_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    g_arr = arr;  // 保存自动重装值

    // 使能定时器3和相关GPIO时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // 配置GPIO - PA6(TIM3_CH1), PA7(TIM3_CH2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置GPIO - PB0(TIM3_CH3), PB1(TIM3_CH4)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 初始化定时器3
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 初始化PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    
    // 配置通道1-4
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    // 使能预装载
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);

    // 使能定时器3
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief  设置PWM占空比
 * @param  channel: PWM通道(0-3)
 * @param  duty: 占空比(0-100)
 * @retval 无
 */
void PWM_Out(u8 channel, u8 duty)
{
    u16 value;
    
    // 限制占空比范围
    if(duty > 100) duty = 100;
    
    // 计算PWM值
    value = (u16)(((float)duty / 100) * g_arr);
    
    // 设置PWM值
    switch(channel)
    {
        case PWM_PA6:
            TIM_SetCompare1(TIM3, value);
            break;
        case PWM_PA7:
            TIM_SetCompare2(TIM3, value);
            break;
        case PWM_PB0:
            TIM_SetCompare3(TIM3, value);
            break;
        case PWM_PB1:
            TIM_SetCompare4(TIM3, value);
            break;
        default:
            break;
    }
}

/**
 * @brief  设置舵机角度
 * @param  channel: PWM通道(0-3)
 * @param  angle: 角度值(0-180)
 * @retval 无
 */
void Servo_SetAngle(u8 channel, u8 angle)
{
    u16 pulse;
    
    // 限制角度范围
    if(angle > SERVO_ANGLE_MAX)
        angle = SERVO_ANGLE_MAX;
    
    // 计算对应的PWM值
    pulse = (u16)(SERVO_PULSE_MIN + (float)(SERVO_PULSE_MAX - SERVO_PULSE_MIN) * angle / SERVO_ANGLE_MAX);
    
    // 设置PWM值
    switch(channel)
    {
        case PWM_PA6:
            TIM_SetCompare1(TIM3, pulse);
            break;
        case PWM_PA7:
            TIM_SetCompare2(TIM3, pulse);
            break;
        case PWM_PB0:
            TIM_SetCompare3(TIM3, pulse);
            break;
        case PWM_PB1:
            TIM_SetCompare4(TIM3, pulse);
            break;
        default:
            break;
    }
}

/**
 * @brief  PWM初始化
 * @param  mode: PWM模式(PWM_MODE_NORMAL/PWM_MODE_SERVO)
 * @retval 无
 */
void PWM_Init(u8 mode)
{
    if(mode == PWM_MODE_SERVO)
    {
        // 舵机模式：50Hz PWM
        TIM3_PWM_Init(19999, 71);  // 72M/72/20000 = 50Hz
        
        // 将所有舵机置于中间位置
        Servo_SetAngle(PWM_PA6, 90);
        Servo_SetAngle(PWM_PA7, 90);
        Servo_SetAngle(PWM_PB0, 90);
        Servo_SetAngle(PWM_PB1, 90);
    }
    else
    {
        // 普通PWM模式：10KHz PWM
        TIM3_PWM_Init(7199, 0);    // 72M/7200 = 10KHz
        
        // 初始占空比设为0
        PWM_Out(PWM_PA6, 0);
        PWM_Out(PWM_PA7, 0);
        PWM_Out(PWM_PB0, 0);
        PWM_Out(PWM_PB1, 0);
    }
}
