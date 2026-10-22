#include "key.h"
#include "delay.h"

static const uint16_t key_gpio[4] = { GPIO_Pin_14, GPIO_Pin_15, GPIO_Pin_12, GPIO_Pin_13 };
static GPIO_TypeDef* const key_port[4] = { GPIOB, GPIOB, GPIOB, GPIOB };

void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    gpio_init_struct.GPIO_Mode = GPIO_Mode_IPU;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;

    for (uint8_t i = 0; i < 4; i++)
    {
        gpio_init_struct.GPIO_Pin = key_gpio[i];
        GPIO_Init(key_port[i], &gpio_init_struct);
    }
}

uint8_t key_scan(void)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        if (!(key_port[i]->IDR & key_gpio[i]))
        {
            delay_ms(5);
            if (!(key_port[i]->IDR & key_gpio[i]))
            {
                return i;
            }
        }
    }

    return 0xFF;
}
