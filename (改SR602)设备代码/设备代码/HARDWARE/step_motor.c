#include "step_motor.h"
#include "delay.h"


static uint8_t motorSequence_8[] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x09};   // 步进电机八拍序列
static uint16_t currentStep_8 = 0; // 目前处于哪一拍
static uint8_t stepx = 0;
static uint8_t step = 0;

static int STEEP_motor[4] = {GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11};
static GPIO_TypeDef *STEEP_PORT[4] = {GPIOB, GPIOB, GPIOB, GPIOB};

/**
 * @brief       步进电机初始化
 * @param       无
 * @retval      无
 */
void step_motor_init(void)
{
	for(int i = 0; i < 4; i++)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
		GPIO_InitStructure.GPIO_Pin  = STEEP_motor[i];
		//上拉输入
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(STEEP_PORT[i], &GPIO_InitStructure);
        GPIO_SetBits(STEEP_PORT[i], STEEP_motor[i]);
	}
} 


/**
 * @brief       刹车
 * @param       无
 * @retval      无
 */
void Move_StopNow(void)
{
    uint8_t stop = 0x0F;
    if (stop & 0x01)
    {
        // GPIO_A_ON4();
        GPIO_ResetBits(STEEP_PORT[0], STEEP_motor[0]);
    }
    else
    {
        // GPIO_A_OFF4();
        GPIO_SetBits(STEEP_PORT[0], STEEP_motor[0]);
    }
    if (stop & 0x02)
    {
        // GPIO_B_ON4();
        GPIO_ResetBits(STEEP_PORT[1], STEEP_motor[1]);
    }
    else
    {
        // GPIO_B_OFF4();
        GPIO_SetBits(STEEP_PORT[1], STEEP_motor[1]);
    }
    if (stop & 0x04)
    {
        // GPIO_C_ON4();
        GPIO_ResetBits(STEEP_PORT[2], STEEP_motor[2]);
    }
    else
    {
        // GPIO_C_OFF4();
        GPIO_SetBits(STEEP_PORT[2], STEEP_motor[2]);
    }
    if (stop & 0x08)
    {
        // GPIO_D_ON4();
        GPIO_ResetBits(STEEP_PORT[3], STEEP_motor[3]);
    }
    else
    {
        // GPIO_D_OFF4();
        GPIO_SetBits(STEEP_PORT[3], STEEP_motor[3]);
    }
}
/**
 * @brief       步进电机转动 连续转动没有抖动
 * @param       dir 方向 1是正向 0是反向
 * @retval      无
 */
void stepMotor_8(uint8_t dir)
{
		if (dir == 1)
				stepx = (stepx + 1) % 8;
    else if (dir == 0)
        stepx = (stepx + 7) % 8;
    step = motorSequence_8[stepx];
    if (step & 0x01)
    {	
        GPIO_ResetBits(STEEP_PORT[0], STEEP_motor[0]);
    }
    else
    {
        GPIO_SetBits(STEEP_PORT[0], STEEP_motor[0]);
    }
    if (step & 0x02)
    {
        GPIO_ResetBits(STEEP_PORT[1], STEEP_motor[1]);
    }
    else
    {
        GPIO_SetBits(STEEP_PORT[1], STEEP_motor[1]);
    }
    if (step & 0x04)
    {
        GPIO_ResetBits(STEEP_PORT[2], STEEP_motor[2]);
    }
    else
    {
        GPIO_SetBits(STEEP_PORT[2], STEEP_motor[2]);
    }
    if (step & 0x08)
    {
        GPIO_ResetBits(STEEP_PORT[3], STEEP_motor[3]);
    }
    else
    {
        GPIO_SetBits(STEEP_PORT[3], STEEP_motor[3]);
    }
}
/**
 * @brief       步进电机转动 连续转动没有抖动
 * @param       dir 方向  方向 1是正向 0是反向
 * @param       Step 步数
 * @retval      里面用到了delay_ms阻塞函数
 */
void Move_Step_8(uint8_t dir, uint16_t Step)
{
    uint16_t Count = Step; // 步数
    while (Count--)
    {
        stepMotor_8(dir);
        delay_ms(2);
    }
}