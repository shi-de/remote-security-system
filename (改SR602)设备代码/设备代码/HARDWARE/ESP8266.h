#ifndef ESP8266_H
#define ESP8266_H

#include "sys.h"
#include "string.h"
#include "stdlib.h"

extern int wifi_flag;
extern int mqtt_flag;

void Net_MQTT_Init(void);
void ESP8266_SendCmd(char *cmd);
void ESP8266_Init(void);
void ESP8266_MQTT_GetMsgAndPara(const char *cmdres, int len);
void Value_register_msgpara(const char *name, int *var_ptr);
void Mqtt_data(void);

#endif