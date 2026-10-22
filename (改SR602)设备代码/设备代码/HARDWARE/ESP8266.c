#include "ESP8266.h"
#include "usart2.h"
#include "delay.h"



//ESP8266初始化状态值
int wifi_flag = 0;
int mqtt_flag = 0;

void Mqtt_data(void)
{
    Value_register_msgpara("wifi_flag", &wifi_flag);
    Value_register_msgpara("mqtt_flag", &mqtt_flag);
}

// 变量映射表
typedef struct {
    const char *name;
    int *var_ptr;
} VarMap;

#define MAX_VARIABLES 20
static VarMap var_map[MAX_VARIABLES];
static int var_count = 0;

/// @brief  注册变量到消息解析
/// @param  name: 变量名 var_ptr: 变量指针
/// @return 无
void Value_register_msgpara(const char *name, int *var_ptr) {
    printf("Registered: %s\r\n", name);
    if (var_count >= MAX_VARIABLES || !name || !var_ptr) {
        return; // 表满或参数无效
    }
    var_map[var_count].name = name;
    var_map[var_count].var_ptr = var_ptr;
    var_count++;
}

/// @brief  串口发送字符串
/// @param  str: 字符串
/// @return 无
void USART2_SendStr(char *str)
{
    // 将字符串放入发送缓冲区
    while (*str)
    {
        while ((USART2->SR & 0X40) == 0)
            ; // 循环发送,直到发送完毕
        USART2->DR = (u8)*str;
        str++;
    }
    while ((USART2->SR & 0X40) == 0)
        ;
    USART2->DR = (u8)'\r';
    // 发送结束 \n
    while ((USART2->SR & 0X40) == 0)
        ;
    USART2->DR = (u8)'\n';
} 	

/// @brief  ESP8266发送命令
/// @param  cmd: 命令
/// @return 无
void ESP8266_SendCmd(char *cmd)
{
    USART2_SendStr(cmd);
    // 发送结束标志符号FFDD
    USART2_SendStr("FFDD");
}

/// @brief  ESP8266解析消息 在接收到消息后调用
/// @param  cmd: 消息
/// @return 无
void ESP8266_MQTT_GetMsgAndPara(const char *cmdres, int len) {
    if (!cmdres || len < 10 || len >= 200) return;
    char cmd[200];
    memcpy(cmd, cmdres, len);
    cmd[len] = '\0';
    if (!strstr(cmd, "TODEVICEDATA")) return;
    // 解析##分隔符
    char *p1 = strstr(cmd, "##");
    if (!p1) return;
    char *p2 = strstr(p1 + 2, "##");
    if (!p2) return;
    char *p3 = strstr(p2 + 2, "##");
    if (!p3) return;
    
    // 提取key/value
    char key[50] = {0}, value[50] = {0};
    int key_len = p2 - p1 - 2;
    int val_len = p3 - p2 - 2;
    if (key_len >= sizeof(key) || val_len >= sizeof(value)) return;
    
    memcpy(key, p1 + 2, key_len);
    memcpy(value, p2 + 2, val_len);
    //打印出key和value
    printf("Mqtt d %s=%s\n", key, value);
    printf("var_count=%d\n", var_count);
    // 查找并赋值
    for (int i = 0; i < var_count; i++) {
        if (strcmp(key, var_map[i].name) == 0) {
            char *endptr;
            long v = strtol(value, &endptr, 10);
            if (*endptr == '\0') { // 转换成功
                *var_map[i].var_ptr = (int)v;
            }
            break;
        }
    }
}

/// @brief  MQTT链接 配置在此修改
/// @param  无
/// @return 无
void Net_MQTT_Init(void)
{
    //等待ESP8266模块 连接上网络
    //等待ESP8266连接上WiFi
    while (1)
    {
        printf("Wifi_flag=%d\rmqtt_flag=%d\n", wifi_flag, mqtt_flag);
        if (wifi_flag == 1 || mqtt_flag == 1)
        {
            break;
        }
        delay_ms(1000);
    }
}


