#ifndef HARDWAREIIC_H
#define HARDWAREIIC_H
#include "sys.h"
#include "stm32f10x.h"
#include "usart1.h"

// 定义调试打印宏
#define I2C_DEBUG 1  // 1:开启调试信息 0:关闭调试信息
#if I2C_DEBUG
    #define I2C_INFO(format, ...) printf("I2C: "format"\r\n", ##__VA_ARGS__)
#else
    #define I2C_INFO(format, ...)
#endif

// I2C相关定义
#define I2Cx I2C1
#define I2C_TIMEOUT 1000  // 超时时间（单位：ms）
#define I2C_RETRY_TIMES 3 // 重试次数

// 错误代码定义
typedef enum {
    I2C_OK = 0,
    I2C_ERROR_BUSY,
    I2C_ERROR_START,
    I2C_ERROR_ADDR,
    I2C_ERROR_DATA,
    I2C_ERROR_STOP,
    I2C_ERROR_TIMEOUT
} I2C_Status;

// 函数声明
void Hardware_I2C_Init(void);
I2C_Status I2C_WriteByte(uint8_t addr, uint8_t data, uint8_t slave_address);
I2C_Status I2C_ReadByte(uint8_t deviceAddress, uint8_t registerAddress, uint8_t* data);
void I2C_ScanAddress(void);
void I2C_Reset(void);

#endif /* HARDWAREIIC_H */
