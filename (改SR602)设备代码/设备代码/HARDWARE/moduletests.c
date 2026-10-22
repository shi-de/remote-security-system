#include "moduletests.h"
#include "oled.h"
#include "delay.h"
#include "key.h"
#include "usart1.h"

#if ENABLE_OLED_TEST
void OLED_DisplayTest(void)
{
    extern const u8 F6x8[];
    extern const u8 F8X16[];
    
    DEBUG_INFO("Starting OLED Display Test...");
    
    OLED_CLS();
    OLED_Print(0, 0, (u8*)"OLED Test", TYPE8X16, TYPE8X16);
    delay_ms(1000);
    OLED_CLS();
    DEBUG_INFO("OLED Display Test Done");
}

void OLED_FillTest(void)
{
    DEBUG_INFO("Starting OLED Graphics Test...");
    OLED_Fill(0xFF);
    delay_ms(10);
    OLED_Fill(0x00);
    delay_ms(10);
    
    DEBUG_INFO("OLED Graphics Test Done");
}

void OLED_ChineseTest(void)
{
    DEBUG_INFO("Starting OLED Chinese Test...");	
    OLED_Print(0, 0, (u8*)"²âÊÔ", TYPE16X16, TYPE8X16);
    DEBUG_INFO("Starting OLED Scroll Test...");
}
#endif

#if ENABLE_KEY_TEST
void KEY_FunctionTest(void)
{
    uint8_t key;
    DEBUG_INFO("Starting KEY Function Test...");
    
    while(1)
    {
        key = key_scan();
        if(key != 0xFF)
        {
            switch(key)
            {
                case KEY_UP:
                    DEBUG_INFO("KEY_UP Pressed");
                    break;
                case KEY_DOWN:
                    DEBUG_INFO("KEY_DOWN Pressed");
                    break;
                case KEY_LEFT:
                    DEBUG_INFO("KEY_LEFT Pressed");
                    break;
                case KEY_RIGHT:
                    DEBUG_INFO("KEY_RIGHT Pressed");
                    break;
                default:
                    break;
            }
        }
        delay_ms(10);
    }
}
#endif

void RunModuleTests(void)
{
    DEBUG_INFO("Starting Module Tests...");
    
#if ENABLE_OLED_TEST
    OLED_DisplayTest();
    OLED_FillTest();
    OLED_ChineseTest();
#endif

#if ENABLE_KEY_TEST
    KEY_FunctionTest();
#endif

#if ENABLE_SENSOR_TEST
    SENSOR_BasicTest();
#endif

    DEBUG_INFO("All Module Tests Completed");
}
