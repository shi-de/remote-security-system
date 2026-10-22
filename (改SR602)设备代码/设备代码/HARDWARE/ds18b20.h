#ifndef __DS18B20_H
#define __DS18B20_H	
#include "sys.h"
#include "misc.h"

#define DS18B20_PORT    GPIOB
#define DS18B20_PIN   GPIO_Pin_5
#define DS18B20_PORT_RCC   RCC_APB2Periph_GPIOB
#define	DS18B20_DQ_OUT PBout(5) //数据端口	
#define	DS18B20_DQ_IN  PBin(5)  //数据端口

void DS18B20_IO_IN(void);
void DS18B20_IO_OUT(void);

u8 DS18B20_Init(void);

float DS18B20_GetTemperture(void);

#endif 