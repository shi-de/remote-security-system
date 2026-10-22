#include "usart3.h"

// 定义全局控制变量
extern volatile u8 Out_Control_Val; 

// 接收缓冲区
char Usart3_RxBuff[200] = {0};

/**
 * @brief  USART3 初始化 (PB10=TX, PB11=RX)
 * @param  bound: 波特率
 */
void usart3_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 时钟使能
    // 注意：USART3 挂载在 APB1 总线上，GPIOB 挂载在 APB2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 2. GPIO 配置
    // USART3_TX  PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // USART3_RX  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 3. USART 参数配置
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

    // 4. NVIC 中断配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 优先级根据需要调整
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 5. 开启接收中断并使能串口
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
}

/**
 * @brief  解析指令函数
 * 格式: TODEVICEDATA##out##Y## 或 TODEVICEDATA##out##N##
 */
void USART3_Parse_Command(char *cmd)
{
    // 1. 首先判断是不是发送给设备的数据
    if (strstr(cmd, "TODEVICEDATA") == NULL) return;

    // 2. 判断是否包含目标 key "out"
    if (strstr(cmd, "##out##") != NULL)
    {
        // 3. 查找 Value 是 Y 还是 N
        // 直接匹配完整子串，防止误判
        if (strstr(cmd, "##out##Y##") != NULL)
        {
            Out_Control_Val = 1; // 设置为 1
            printf("Set Out: ON (1)\r\n"); // 调试用
        }
        else if (strstr(cmd, "##out##N##") != NULL)
        {
            Out_Control_Val = 0; // 设置为 0
            printf("Set Out: OFF (0)\r\n"); // 调试用
        }
    }
}

/**
 * @brief  USART3 中断服务函数
 */
void USART3_IRQHandler(void)
{
    u8 Res;
    static u8 RxCount = 0;

    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        Res = USART_ReceiveData(USART3);
        
        // 接收逻辑：以 '\n' 结尾，或者单纯依靠 buffer 长度防止溢出
        // 这里兼容你原来代码的 '\n' 结束符逻辑
        if (Res == '\n' || Res == '\r') // 遇到换行符处理
        {
            if (RxCount > 5) // 简单的长度过滤，防止空行
            {
                Usart3_RxBuff[RxCount] = '\0'; // 添加字符串结束符
                
                // 执行解析
                USART3_Parse_Command(Usart3_RxBuff);
            }
            // 清空计数和缓冲区
            RxCount = 0;
            memset(Usart3_RxBuff, 0, sizeof(Usart3_RxBuff));
        }
        else
        {
            if (RxCount < 199) // 防止溢出
            {
                Usart3_RxBuff[RxCount++] = Res;
            }
            else // 缓冲区满，强制清空重新接收
            {
                RxCount = 0;
                memset(Usart3_RxBuff, 0, sizeof(Usart3_RxBuff));
            }
        }
    }
}