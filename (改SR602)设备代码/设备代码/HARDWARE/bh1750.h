#ifndef __BH1750_H
#define __BH1750_H


#include "sys.h"
#include "delay.h"

void 		BH1750_WriteByte				(uint8_t addr);
uint16_t 	BH1750_Read_Data				(void);
void 		BH1750_Power_ON					(void);
void 		BH1750_Power_OFF				(void);
void 		BH1750_RESET					(void);
void 		BH1750_Init						(void);
float 	Light_Intensity					(void);
		
#endif

