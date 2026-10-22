/************************* Header Files *************************/
#include "sys.h"
#include "stdlib.h"
#include "delay.h"
#include "usart1.h"
#include "usart2.h"
#include "key.h"
#include "oled.h"
#include "hardwareIiic.h"
#include "moduletests.h"
#include "adc_in.h"
#include "digital_in.h"
#include "digital_out.h"
#include "tim_out.h"
#include "beep.h"
#include "led.h"
#include "ds18b20.h"
#include "bh1750.h"
#include "aht10.h"
#include "dht11.h"
#include "step_motor.h"
#include "ESP8266.h"
#include "uid_read.h"
#include "MFRC522.h"

/************************* Defines *************************/
#define SYSTEM_VERSION    "V1.0.0"
#define LOOP_DELAY_MS    1
#define PAGE_AUTO_SWITCH_TIME  20  // 页面自动切换时间(ms)
int MqttLoopCount = 0;
#define UPDATA_TOMQTT_TIME 250
#define ITEMS_PER_PAGE       4   // 每页显示项数
int current_page_num = 0;
// 当前页码
static u8 current_page = 0;
static u8 total_pages = 0;
/************************* Global Variables *************************/
static uint32_t g_system_time = 0;    // System running time counter
static uint32_t last_page_switch_time = 0;  // 上次页面切换时间

u8 str[32] = {0}; // 显示的字符串s
char Mqttmsg[256];       // JSON 缓冲区
// 显示项结构体
typedef struct {
    char name[10];        // 显示名称
    int value;          // 显示值
    char unit[5];       // 单位
} DISPLAY_ITEM;

/************************* Function *********************************/
static void System_Init(void);
static void Value_register(void);
static void Main_Loop(void);
static void Main_Gui(void);
static void Display_Page(u8 page);
static void Get_Process_Data(void);
static void Switch_Page(void);
void BuildSensorJson();
int Flash_Key_Data();


/**
 * DHT11 PA6 和 PWM PA6 冲突 不能同时开
 * V1.0 版本
 * PB0 和 PB1 用于PWM 但是目前用于输出继电器
 */