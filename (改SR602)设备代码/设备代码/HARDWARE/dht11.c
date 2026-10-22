#include "dht11.h"
#include "delay.h" // 务必包含你的延时函数头文件

/**
 * @brief  配置DHT11引脚为输出模式
 */
static void DHT11_Mode_Out_PP(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

/**
 * @brief  配置DHT11引脚为输入模式
 */
static void DHT11_Mode_IPU(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

/**
 * @brief  复位DHT11 (主机发送开始信号)
 */
static void DHT11_Rst(void)
{
    DHT11_Mode_Out_PP();      // 切换为输出
    GPIO_ResetBits(DHT11_PORT, DHT11_PIN); // 拉低总线
    delay_ms(20);             // 拉低至少18ms
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);   // 拉高总线
    delay_us(30);             // 主机拉高20~40us
}

/**
 * @brief  等待DHT11的回应
 * @return 0:存在, 1:未检测到
 */
static u8 DHT11_Check(void)
{
    u8 retry = 0;
    DHT11_Mode_IPU(); // 切换为输入
    
    // 等待DHT11拉低 (DHT11会拉低80us作为响应)
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == 1 && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    if (retry >= 100) return 1; // 超时未拉低
    else retry = 0;

    // 等待DHT11拉高 (DHT11拉低80us后会拉高80us)
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == 0 && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    if (retry >= 100) return 1; // 超时未拉高
    
    return 0;
}

/**
 * @brief  从DHT11读取一个位
 * @return 1/0
 */
static u8 DHT11_Read_Bit(void)
{
    u8 retry = 0;
    // 等待变为低电平 (上一位数据的结束)
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == 1 && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    retry = 0;
    
    // 等待变为高电平 (数据位开始，低电平持续50us)
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == 0 && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    
    delay_us(40); // 等待40us用于判断是0还是1
    // 0的信号高电平持续26-28us，1的信号高电平持续70us
    
    if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == 1)
        return 1;
    else
        return 0;
}

/**
 * @brief  从DHT11读取一个字节
 */
static u8 DHT11_Read_Byte(void)
{
    u8 i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= DHT11_Read_Bit();
    }
    return dat;
}

/**
 * @brief  初始化DHT11
 * @return 0:成功, 1:失败
 */
u8 DHT11_Init(void)
{
    RCC_APB2PeriphClockCmd(DHT11_PORT_RCC, ENABLE); // 开启时钟
    DHT11_Rst();  // 复位
    return DHT11_Check(); // 等待响应
}

/**
 * @brief  读取一次数据
 * @param  temp: 存储温度值的指针 (0-50°C)
 * @param  humi: 存储湿度值的指针 (20-90%RH)
 * @return 0:成功, 1:失败
 */
u8 DHT11_Read_Data(u8 *temp, u8 *humi)
{
    u8 buf[5];
    u8 i;
    
    DHT11_Rst();
    if (DHT11_Check() == 0) // 检测到响应
    {
        for (i = 0; i < 5; i++)
        {
            buf[i] = DHT11_Read_Byte();
        }
        
        // 校验数据: 前4个字节之和 = 第5个字节(校验和)
        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0]; // 湿度整数部分
            *temp = buf[2]; // 温度整数部分
            // buf[1]和buf[3]是小数部分，DHT11通常为0，此处忽略
            return 0;
        }
    }
    return 1;
}