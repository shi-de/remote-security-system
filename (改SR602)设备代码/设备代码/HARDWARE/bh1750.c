#include "bh1750.h"
#include "hardwareIiic.h"
#include "delay.h"

/*	器件地址(7位-用左移1位-最后1位为读写位--1为读-0为写) */
#define				BH1750_Addr_GND_REG				0X23	//ADDR引脚接低电平
#define				BH1750_Addr_VCC_REG				0X5C	//ADDR引脚接高电平


				/******	指令集宏定义*****/
#define				BH1750_Power_OFF_REG			0x00	//断电指令
#define				BH1750_Power_ON_REG				0x01	//通电,等待测量命令
#define				BH1750_MODULE_RESET_REG			0x07	//重置数据寄存器,关机模式下，不能接受重置命令

/*	器件地址(7位-用左移1位-最后1位为读写位--1为读-0为写) */
#define				BH1750_Addr_GND_REG				0X23	//ADDR引脚接低电平
#define				BH1750_Addr_VCC_REG				0X5C	//ADDR引脚接高电平


				/******	指令集宏定义*****/
#define				BH1750_Power_OFF_REG			0x00	//断电指令
#define				BH1750_Power_ON_REG				0x01	//通电,等待测量命令
#define				BH1750_MODULE_RESET_REG			0x07	//重置数据寄存器,关机模式下，不能接受重置命令


/*************************************************	
	不同模式下分辨率不同（也即精度不同）
	高分辨率模式2：分辨率是0.5lx
	高分辨率模式：分辨率1lx
	低分辨率模式：分辨率4lx
	不同模式只是精度不一样，对于计算没有区别
***************************************************/
	/**	工作模式指令集	**/
#define				BH1750_CONTINUE_H_MODE			0x10	//连续H分辨率模式
#define				BH1750_CONTINUE_H_MODE2			0x11	//连续H分辨率模式2
#define				BH1750_CONTINUE_L_MODE			0x13	//连续L分辨率模式
#define				BH1750_ONE_TIME_H_MODE			0x20	//一次H分辨率模式
#define				BH1750_ONE_TIME_H_MODE2			0x21	//一次H分辨率模式2
#define				BH1750_ONE_TIME_L_MODE			0x23	//一次性L分辨率模式
#define				BH1750_CONTINUE_H_MODE			0x10	//连续H分辨率模式
#define				BH1750_CONTINUE_H_MODE2			0x11	//连续H分辨率模式2
#define				BH1750_CONTINUE_L_MODE			0x13	//连续L分辨率模式
#define				BH1750_ONE_TIME_H_MODE			0x20	//一次H分辨率模式
#define				BH1750_ONE_TIME_H_MODE2			0x21	//一次H分辨率模式2
#define				BH1750_ONE_TIME_L_MODE			0x23	//一次性L分辨率模式

void BH1750_WriteByte(uint8_t addr)
{

    /*	先判断总线是否忙  */
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

    /* 发送Start信号,切换到主模式（M/SL 位置 1） */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /*	等待EV5事件,IIC开始信号已经发出 */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /*	发送器件地址,发送方向: I2C_Direction_Transmitter */
    I2C_Send7bitAddress(I2Cx, (BH1750_Addr_GND_REG<<1|0), I2C_Direction_Transmitter);
    /*	等待EV6事件：表示地址已经发送  */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    /*	发送操作的寄存器地址  */
    I2C_SendData(I2Cx, addr);
    /*	等待EV8事件：数据寄存器DR为空,地址数据已经发送	*/
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	
	    /*	产生应答信号	*/
    I2C_AcknowledgeConfig(I2Cx, ENABLE);

    I2C_GenerateSTOP(I2Cx, ENABLE);//产生停止信号,关闭I2Cx总线
}


/****************************************************************************
**	函数名称：BH1750_Read_Data()
**	函数功能：BH1750 读取数据
**	参数说明：
**			uint8_t addr ：寄存器地址
**
**	返回值：RX_Data
**	IIC函数说明：
**				I2C_GetFlagStatus（IIC_x, IIC_Flag_x）-- 检查标志位是否置 1
**
*****************************************************************************/
uint16_t BH1750_Read_Data(void)
{
    uint8_t 	H_Data;							//高 8 位数据
		uint8_t 	L_Data;							//低 8 位 数据
		uint16_t 	Rx_Data;						//完整 16 位数据

    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));					/* 先判断总线是否忙 */

    I2C_GenerateSTART(I2Cx, ENABLE); 								/* 发送Start信号,切换到主模式  */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));		/* 等待EV5事件,IIC开始信号已经发出 */

    /*	发送读地址指令,发送方向: I2C_Direction_Receiver */
    I2C_Send7bitAddress(I2Cx, BH1750_Addr_GND_REG<<1|1, I2C_Direction_Receiver);
    /*	等待EV6事件：地址已发送 (接收类型)  */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
    
    I2C_AcknowledgeConfig(I2Cx, ENABLE);							/*	产生应答信号	*/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)); 	/* 等待EV7事件 */
    H_Data = I2C_ReceiveData(I2Cx);									/* 读取高8位数据  */

    
    I2C_AcknowledgeConfig(I2Cx, ENABLE);							/* 产生应答信号 */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));	/*	等待EV7事件 */
		/* 与高8位合成16位完整数据 */
		L_Data = I2C_ReceiveData(I2Cx);									/* 读取低8位 */
	   
    I2C_AcknowledgeConfig(I2Cx, DISABLE); 							/*	产生非应答信号	*/
		I2C_GenerateSTOP(I2Cx, ENABLE);									/**	发送停止信号	**/
    
    Rx_Data = ((uint16_t)H_Data << 8) + L_Data;						/* 16位数据合成 */

    return Rx_Data;
}


/**	BH1750上电 **/
void BH1750_Power_ON(void)
{
	BH1750_WriteByte(BH1750_Power_ON_REG);
}

/**	BH1750断电 **/
void BH1750_Power_OFF(void)
{
	BH1750_WriteByte(BH1750_Power_OFF_REG);
}

/**	BH1750数据寄存器复位 **/
void BH1750_RESET(void)
{
	BH1750_WriteByte(BH1750_MODULE_RESET_REG);
}

/**	BH1750初始化 **/
void BH1750_Init(void)
{
  BH1750_Power_OFF();
	BH1750_Power_ON();
	BH1750_RESET();
	BH1750_WriteByte(BH1750_CONTINUE_H_MODE2);
}

/**	获取光照强度 **/
/**	分辨率	光照强度（单位lx）=（High Byte + Low Byte）/ 1.2  **/
float Light_Intensity(void)
{
	return (float)(BH1750_Read_Data() / 1.2f );
}
