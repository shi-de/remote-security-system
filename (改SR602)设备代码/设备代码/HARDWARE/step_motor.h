#ifndef _STEP_MOTOR1_H
#define _STEP_MOTOR1_H

#include "sys.h"
/******************************************************************************************/

/* 外部接口函数*/
void step_motor_init(void);
void Move_StopNow(void);
void stepMotor_8(uint8_t dir);
void Move_Step_8(uint8_t dir, uint16_t Step);

#endif