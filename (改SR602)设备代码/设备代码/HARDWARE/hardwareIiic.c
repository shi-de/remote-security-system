#include "hardwareIiic.h"
#include "delay.h"

/// @brief  等待I2C事件，带超时
/// @param  event: 要等待的事件
/// @return I2C_Status: 操作状态
static I2C_Status I2C_WaitEvent(uint32_t event)
{
    uint32_t timeout = I2C_TIMEOUT;
    while(!I2C_CheckEvent(I2Cx, event))
    {
        if(timeout-- == 0)
        {
            I2C_INFO("Event 0x%08X timeout", event);
            return I2C_ERROR_TIMEOUT;
        }
        delay_us(1);
    }
    return I2C_OK;
}

/// @brief  硬件I2C初始化
void Hardware_I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
    
    I2C_INFO("I2C Init Start");
    
    // 复位I2C
    I2C_Reset();
    
    // 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置I2C
    I2C_DeInit(I2Cx);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 1200000;
    
    I2C_Init(I2Cx, &I2C_InitStructure);
    I2C_Cmd(I2Cx, ENABLE);
    
    I2C_INFO("I2C Init Complete");
}

/// @brief  I2C复位函数
void I2C_Reset(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // I2C_INFO("I2C Reset Start");
    
    // 关闭I2C
    I2C_Cmd(I2Cx, DISABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
    delay_ms(50);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
    
    // 配置为普通GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 手动产生9个时钟清除总线
    for(int i = 0; i < 9; i++)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_6);
        delay_us(5);
        GPIO_ResetBits(GPIOB, GPIO_Pin_6);
        delay_us(5);
    }
    
    // I2C_INFO("I2C Reset Complete");
}

/// @brief  I2C写入一个字节
/// @return I2C_Status: 操作状态
I2C_Status I2C_WriteByte(uint8_t addr, uint8_t data, uint8_t slave_address)
{
    uint8_t retry = 0;
    I2C_Status status;
    
    while(retry++ < I2C_RETRY_TIMES)
    {
        // 等待总线空闲
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        {
            I2C_INFO("Bus busy, retry %d", retry);
            delay_ms(2);
            continue;
        }
        
        // 发送起始信号
        I2C_GenerateSTART(I2Cx, ENABLE);
        status = I2C_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
        if(status != I2C_OK) continue;
        
        // 发送从机地址
        I2C_Send7bitAddress(I2Cx, slave_address, I2C_Direction_Transmitter);
        status = I2C_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
        if(status != I2C_OK) continue;
        
        // 发送寄存器地址
        I2C_SendData(I2Cx, addr);
        status = I2C_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
        if(status != I2C_OK) continue;
        
        // 发送数据
        I2C_SendData(I2Cx, data);
        status = I2C_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
        if(status != I2C_OK) continue;
        
        // 发送停止信号
        I2C_GenerateSTOP(I2Cx, ENABLE);
        delay_us(100);
        
        // I2C_INFO("Write Success: addr=0x%02X, data=0x%02X", addr, data);
        return I2C_OK;
    }
    
    I2C_INFO("Write Failed after %d retries", I2C_RETRY_TIMES);
    return status;
}

/// @brief  I2C读取一个字节
/// @return I2C_Status: 操作状态
I2C_Status I2C_ReadByte(uint8_t deviceAddress, uint8_t registerAddress, uint8_t* data)
{
    uint8_t retry = 0;
    I2C_Status status;
    
    while(retry++ < I2C_RETRY_TIMES)
    {
        // 等待总线空闲
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        {
            delay_ms(2);
            continue;
        }
        
        // 发送起始信号
        I2C_GenerateSTART(I2Cx, ENABLE);
        status = I2C_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
        if(status != I2C_OK) continue;
        
        // 发送设备地址(写)
        I2C_Send7bitAddress(I2Cx, deviceAddress, I2C_Direction_Transmitter);
        status = I2C_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
        if(status != I2C_OK) continue;
        
        // 发送寄存器地址
        I2C_SendData(I2Cx, registerAddress);
        status = I2C_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
        if(status != I2C_OK) continue;
        
        // 重新发送起始信号
        I2C_GenerateSTART(I2Cx, ENABLE);
        status = I2C_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
        if(status != I2C_OK) continue;
        
        // 发送设备地址(读)
        I2C_Send7bitAddress(I2Cx, deviceAddress, I2C_Direction_Receiver);
        status = I2C_WaitEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
        if(status != I2C_OK) continue;
        
        // 禁用ACK，准备接收最后一个字节
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
        
        // 发送停止信号
        I2C_GenerateSTOP(I2Cx, ENABLE);
        
        // 等待接收数据
        status = I2C_WaitEvent(I2C_EVENT_MASTER_BYTE_RECEIVED);
        if(status != I2C_OK) continue;
        
        *data = I2C_ReceiveData(I2Cx);
        
        // 重新使能ACK
        I2C_AcknowledgeConfig(I2Cx, ENABLE);
        
        // I2C_INFO("Read Success: addr=0x%02X, data=0x%02X", registerAddress, *data);
        return I2C_OK;
    }
    
    I2C_INFO("Read Failed after %d retries", I2C_RETRY_TIMES);
    return status;
}

/// @brief  I2C地址扫描
void I2C_ScanAddress(void)
{
    uint8_t address;
    uint8_t found = 0;
    
    I2C_INFO("Scanning I2C addresses...");
    
    for(address = 1; address < 128; address++)
    {
        // 等待总线空闲
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        {
            delay_ms(2);
            continue;
        }
        
        // 发送起始信号
        I2C_GenerateSTART(I2Cx, ENABLE);
        if(I2C_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT) != I2C_OK)
        {
            I2C_GenerateSTOP(I2Cx, ENABLE);
            continue;
        }
        
        // 发送设备地址
        I2C_Send7bitAddress(I2Cx, address << 1, I2C_Direction_Transmitter);
        delay_us(100);
        
        // 检查是否有应答
        if(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == SUCCESS)
        {
            I2C_INFO("Found device at address: 0x%02X", address);
            found++;
        }
        
        // 发送停止信号
        I2C_GenerateSTOP(I2Cx, ENABLE);
        delay_ms(2);
    }
    
    I2C_INFO("Scan complete, found %d devices", found);
}