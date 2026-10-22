#include "adc_in.h"
#include "delay.h"

/**
 * @brief  ADC相关GPIO初始化
 * @param  无
 * @retval 无
 */
void ADC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 配置PA0/PA1/PA4/PA5为模拟输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_0  | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief  ADC1初始化
 * @param  无
 * @retval 无
 */
void ADC1_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    
    // 初始化ADC1相关的GPIO
    ADC_GPIO_Init();
    
    // 使能ADC1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    // 复位ADC1
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
    
    // 配置ADC1
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                // 独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                    // 单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;             // 单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;          // 数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                         // 1个转换通道
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // 使能ADC1
    ADC_Cmd(ADC1, ENABLE);
    
    // ADC校准
    ADC_ResetCalibration(ADC1);                 // 复位校准寄存器
    while(ADC_GetResetCalibrationStatus(ADC1)); // 等待复位校准结束
    ADC_StartCalibration(ADC1);                 // 开始校准
    while(ADC_GetCalibrationStatus(ADC1));      // 等待校准结束
}

/**
 * @brief  获取ADC1指定通道的转换值
 * @param  ch: ADC通道号
 * @retval ADC转换值(0-4095)
 */
u16 Get_ADC1_Value(u8 ch)
{
    // 设置指定ADC的规则组通道，设置采样时间
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
    
    // 启动软件转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // 等待转换结束
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    
    // 返回最近一次ADC1规则组的转换结果
    return ADC_GetConversionValue(ADC1);
}

/**
 * @brief  获取ADC1 PA0通道的转换值
 * @param  无
 * @retval ADC转换值
 */
u16 ADC_PA0_GetValue(void)
{
    return Get_ADC1_Value(0);
}

/**
 * @brief  获取ADC1 PA1通道的转换值
 * @param  无
 * @retval ADC转换值
 */
u16 ADC_PA1_GetValue(void)
{
    return Get_ADC1_Value(1);
}

/**
 * @brief  获取ADC1 PA4通道的转换值
 * @param  无
 * @retval ADC转换值
 */
u16 ADC_PA4_GetValue(void)
{
    return Get_ADC1_Value(4);
}

/**
 * @brief  获取ADC1 PA5通道的转换值
 * @param  无
 * @retval ADC转换值
 */
u16 ADC_PA5_GetValue(void)
{
    return Get_ADC1_Value(5);
}
