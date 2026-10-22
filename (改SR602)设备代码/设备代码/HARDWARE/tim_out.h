#ifndef __TIM_OUT_H
#define __TIM_OUT_H

#include "sys.h"

// PWM输出引脚定义
#define PWM_PA6    0  // TIM3_CH1
#define PWM_PA7    1  // TIM3_CH2
#define PWM_PB0    2  // TIM3_CH3  
#define PWM_PB1    3  // TIM3_CH4

// PWM模式定义
#define PWM_MODE_NORMAL  0  // 普通PWM模式(0-100占空比)
#define PWM_MODE_SERVO   1  // 舵机模式(0-180角度)

// 舵机参数定义
#define SERVO_ANGLE_MIN     0    // 最小角度
#define SERVO_ANGLE_MAX     180  // 最大角度
#define SERVO_PULSE_MIN     500  // 对应最小角度的PWM值(0.5ms)
#define SERVO_PULSE_MAX     2500 // 对应最大角度的PWM值(2.5ms)

// 函数声明
void TIM3_PWM_Init(u16 arr, u16 psc);  // 定时器3 PWM初始化
void PWM_Out(u8 channel, u8 duty);      // 设置PWM占空比(0-100)
void Servo_SetAngle(u8 channel, u8 angle);  // 设置舵机角度(0-180)
void PWM_Init(u8 mode);                 // PWM初始化(可选模式)

#endif
