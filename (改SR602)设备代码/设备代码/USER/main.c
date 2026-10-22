#include "main.h"

#define SUPPORT_APP         1   // 1 支持WIFI

/************************User Defines *************************/
/************************************************************ */

#define MAX_DISPLAY_ITEMS    4   // 最大显示项数
DISPLAY_ITEM display_items[MAX_DISPLAY_ITEMS] = {
    {"GateST", 0, "F/O"},
    {"MQ2", 0, "%"},
		{"SR602", 0, "SED"},
    {"Flame",0,"Y/N"},
};
int MQMax = 80;
//注册阈值到消息解析
static void Value_register(void)
{
    Value_register_msgpara("MQMax", &MQMax);
}

/************************* Init Functions *************************/
static void System_Init(void)
{
    SystemInit();
    key_init();
    delay_init();
    uart1_init(115200);
    uart2_init(115200);
    Mqtt_data();
    Value_register();
    DEBUG_INFO("System Init");
    Hardware_I2C_Init();
    OLED_Init();
    DHT11_Init();
    ADC_GPIO_Init();
    Digital_In_Init();
    ADC1_Init();
    Digital_Out_Init();
    BEEP_Init();
    DEBUG_INFO("System Init Done");
}


/************************* Main Function *************************/
int main(void)
{    
    System_Init();
    // 计算总页数
    total_pages = (MAX_DISPLAY_ITEMS + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
    DEBUG_INFO("System Start, Version: %s", SYSTEM_VERSION);
    OLED_CLS();
    if (SUPPORT_APP == 1) { // 支持WIFI
        sprintf(str, "Mqtt init...");
        OLED_Print(0, 0, str, TYPE16X16, TYPE8X16);
        sprintf(str, " - Open WIFI -");
        OLED_Print(0, 16, str, TYPE16X16, TYPE8X16);
        sprintf(str, "Name:test");
        OLED_Print(0, 32, str, TYPE16X16, TYPE8X16);
        sprintf(str, "PSW:123456789");
        OLED_Print(0, 48, str, TYPE16X16, TYPE8X16);
        Net_MQTT_Init();
        OLED_CLS();
        sprintf(str, "Mqtt OK");
        OLED_Print(0, 16, str, TYPE16X16, TYPE8X16);
        delay_ms(1000);
    }
	OLED_CLS();
    while(1)
    {
        delay_ms(LOOP_DELAY_MS);
        Get_Process_Data();
        Main_Gui();
    }
}

static void Get_Process_Data(void)
{
    unsigned char fire_alarm;
    unsigned char intrusion_alarm;

    display_items[1].value = ADC_PA0_GetValue()*100/4096;
    display_items[0].value = PAin(12);
    display_items[3].value = PBin(8);
    display_items[2].value = PBin(9);
    fire_alarm = (display_items[1].value > MQMax && display_items[3].value);
    intrusion_alarm = (display_items[2].value && !display_items[0].value);
//		PBout(4) = 1; //照明
//		PBout(3) = 1; //水泵
		//烟雾浓度超过阈值且检测到火的时候 打开水泵
		if(fire_alarm){
				PBout(4) = 0; //水泵
		}
		else{
				PBout(4) = 1; //水泵
		}
		if(intrusion_alarm)
		{
				PBout(3) = 0; //照明
		}else{
				PBout(3) = 1; //照明
		}

    //数据上传
    if(fire_alarm || intrusion_alarm)
        BEEP_ON();
    else
        BEEP_OFF();

    if(MqttLoopCount++ > UPDATA_TOMQTT_TIME)
		{
				MqttLoopCount= 0 ;
				BuildSensorJson();
				ESP8266_SendCmd(Mqttmsg);
				printf("MQTT JSON: %s\n",Mqttmsg);
		}

}

int page_num_Max = 1;
static void Main_Gui(void)
{
	Flash_Key_Data();
    if(current_page_num == 0)
    {
        current_page = (current_page + 1) % total_pages;
        Display_Page(current_page);
        for(int i = 0; i < PAGE_AUTO_SWITCH_TIME ; i++)
        {
            if(Flash_Key_Data())
            {
                break;
            }
            delay_ms(1);
        }
        if(total_pages>1) OLED_CLS();
    }
    if(current_page_num == 0)
    {
    }
    if(current_page_num == 1)
    {
        uint8_t key = key_scan();
        sprintf(str, "%s", "SetMQMax");
        OLED_Print(2, 0, str, TYPE16X16, TYPE8X16);
        memset(str, 16, 0);
        sprintf(str, "up: ++");
        OLED_Print(2, 16, str, TYPE16X16, TYPE8X16);
        memset(str, 16, 0);
        sprintf(str, "Now:%d   ", MQMax);
        OLED_Print(2, 32, str, TYPE16X16, TYPE8X16);
        memset(str, 16, 0);
        sprintf(str, "du: --");
        OLED_Print(2, 48,                                 str, TYPE16X16, TYPE8X16);
        memset(str, 16, 0);
        if (key == KEY_UP)
        {
            MQMax += 2;
            
        }
        if (key == KEY_DOWN)
        {
            MQMax -= 2;
            if(MQMax < 0)
            {
                MQMax = 0;
            }
        }
    }
}





















/************************* Utils Functions *************************/
/* JSON 生成函数 */
void BuildSensorJson() {
    char* ptr = Mqttmsg;
    int remaining = sizeof(Mqttmsg);
    int len;
    uint8_t first_item = 1;

    len = snprintf(ptr, remaining, "{");
    if (len <= 0 || len >= remaining) goto error_handle;
    ptr += len;
    remaining -= len;

    for (int i = 0; i < MAX_DISPLAY_ITEMS; i++) {
        DISPLAY_ITEM* data = &display_items[i];
        
        if (1) {
            
            if (!first_item) {
                len = snprintf(ptr, remaining, ",");
                if (len <= 0 || len >= remaining) goto error_handle;
                ptr += len;
                remaining -= len;
            }

            len = snprintf(ptr, remaining, 
                          "\"%s\":{\"val\":%d,\"unit\":\"%s\"}",
                          data->name, data->value, data->unit);
            if (len <= 0 || len >= remaining) goto error_handle;
            
            ptr += len;
            remaining -= len;
            first_item = 0;
        }
    }

    /* 闭合括号 */
    if (!first_item) {
        len = snprintf(ptr, remaining, ",");
        if (len <= 0 || len >= remaining) goto error_handle;
        ptr += len;
        remaining -= len;
    }

    len = snprintf(ptr, remaining,
                  "\"MQMax\":{\"val\":%d,\"unit\":\"%%\"}",
                  MQMax);
    if (len <= 0 || len >= remaining) goto error_handle;
    ptr += len;
    remaining -= len;

    len = snprintf(ptr, remaining, "}");
    if (len <= 0 || len >= remaining) goto error_handle;
    return;

error_handle:  // 错误处理（缓冲区不足）
    strncpy(Mqttmsg, "{}", sizeof(Mqttmsg));
}

int Flash_Key_Data()
{
		uint8_t key = key_scan();
		if(key == KEY_LEFT)
    {
        current_page_num--;
				if(current_page_num < 0) current_page_num = 0;
				OLED_CLS();
				return 1;
    }
		if(key == KEY_RIGHT)
    {
        current_page_num++;
				if(current_page_num > page_num_Max) current_page_num = page_num_Max;
				OLED_CLS();
				return 1;
    }
		return 0;
}


static void Display_Page(u8 page)
{
    u8 start_item = page * ITEMS_PER_PAGE;
    u8 end_item = start_item + ITEMS_PER_PAGE;
    u8 i, y_pos;
    
    if(end_item > MAX_DISPLAY_ITEMS)
        end_item = MAX_DISPLAY_ITEMS;
    
    for(i = start_item; i < end_item; i++)
    {
        y_pos = (i - start_item) * 16;  // 每项占16像素高度
        
        if(display_items[i].unit[0] != '\0')
            sprintf(str, "%s:%5d%s", display_items[i].name, display_items[i].value, display_items[i].unit);
        else
            sprintf(str, "%s:%d", display_items[i].name, display_items[i].value);
            
        OLED_Print(0, y_pos, str, TYPE16X16, TYPE8X16);
    }
    
}

