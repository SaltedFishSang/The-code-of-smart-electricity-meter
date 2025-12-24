#include "BSP.h"
u8 g_Equipment_number[17] = "1210012509200001";
/*!
    \brief      RCC_Configuration(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void RCC_Configuration(void)
{
    RCC_ConfigHse(RCC_HSE_ENABLE);
    while (RCC_WaitHseStable() == ERROR)
        ;
    RCC_ConfigSysclk(RCC_SYSCLK_SRC_HSE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_USART1, ENABLE);
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART2, ENABLE);
    RCC_ConfigLpuartClk(RCC_LPUART1CLK, RCC_LPUARTCLK_SRC_APB1);
    RCC_EnableLpuartClk(ENABLE);
}

/**
 * @brief  Configure the nested vectored interrupt controller.
 */
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;
    /* Enable the TIM global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*Set key input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

u8 ledflag = 0;

void LedFlag(void)
{
    ledflag = 1;
    Vtimer_SetTimer(VTIM0, 250 * (3 - 2 * get_mqttlink()), LedFlag);
}

void LedTask(void)
{
    static u8 state = 0;
    if (ledflag)
    {
        ledflag = 0;
        state++;
        if (state == 1)
        {
            LedWorkOn;
        }
        if (state >= 2)
        {
            state = 0;
            LedWorkOff;
        }
    }
}

/*!
    \brief      LedInit(void) LED GPIO口初始化
    \param[in]  None
    \param[out] none
    \retval     none
*/
void LedInit(void)
{
    /*
     *PA6----LEDRUN
     */
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    /*
     *PB0----Alarm RUN
     */
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    /*初始化的时候关闭两个灯*/
    LedWorkOff;
    LedAlarmOff;
    Vtimer_SetTimer(VTIM0, 500, LedFlag);
}

void Dog_Feed(void);

/*!
    \brief     EnableDog:打开硬件看门狗 WDI GPIO设置成推挽输出模式
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EnableDog(void)
{
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
    Vtimer_SetTimer(VTIM1, 30, Dog_Feed);
}

unsigned char feeddog = 0;

/*!
    \brief     Dog_Feed
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Dog_Feed(void)
{
    static uint16_t timecnt = 0;
    static uint8_t en_feed = 1;
    static uint8_t status = 0;
    if (en_feed)
    {
        if (status)
        {
            GPIO_ResetBits(GPIOA, GPIO_PIN_1);
        }
        else
        {
            GPIO_SetBits(GPIOA, GPIO_PIN_1);
        }
        status = ~status;
    }
    if (timecnt >= 1000)
        timecnt = 0;
    else
        timecnt++;

    if (timecnt == 0)
        feeddog = 0;
    else if ((timecnt == 999) && (feeddog == 0))
        en_feed = 0;
    Vtimer_SetTimer(VTIM1, 30, Dog_Feed);
}

/*!
    \brief     Dog
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Dog(void)
{
    feeddog = 1;
}

/*!
    \brief      Key_gpioInit(void) 轻触GPIO口初始化
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Key_gpioInit(void)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Pull = GPIO_PULL_UP;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
    GPIO_ConfigEXTILine(GPIOA_PORT_SOURCE, GPIO_PIN_SOURCE2);
    /*Configure key EXTI line*/
    EXTI_InitStructure.EXTI_Line = EXTI_LINE1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);
    EXTI_ClrITPendBit(EXTI_LINE1);
}
/*!
    \brief      ModeInput_gpioInit(void) 本地远程模式选择开关
    \param[in]  None
    \param[out] none
    \retval     none
*/
void ModeInput_gpioInit(void)
{
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Pull = GPIO_PULL_UP;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
}
/*!
    \brief      EXTI0_1_IRQHandler(void) 按键中断函数
    \param[in]  None
    \param[out] none
    \retval     none
*/
void EXTI0_1_IRQHandler(void)
{
    if (RESET != EXTI_GetITStatus(EXTI_LINE1))
    {
        EXTI_ClrITPendBit(EXTI_LINE1);
        if ((Modbus_ReadDI(3) == 0) && (Modbus_ReadDI(2) == 0)) // 手动模式下  并且没有远程锁扣的情况下 才可以一键分合闸
        {
            Modbus_WirteDI(4, 0);
            if (Modbus_ReadDI(1) == 1)
            {
                RelayControl(0);
                Modbus_WirteDI(1, 0);      // 分闸
                Modbus_WrieteInput(10, 9); // 再写入输入寄存器 最后一次分闸原因  本地
                Flash_WirteModbusDI();
            }
            else
            {
                RelayControl(1);
                Modbus_WirteDI(1, 1); // 合闸
                Flash_WirteModbusDI();
            }
        }
    }
}
/*!
    \brief      ModeInput_Check(void) ModeInput_Check
    \param[in]  None
    \param[out] none
    \retval     none
*/
void ModeInput_Check(void)
{
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_13) == 1)
    {
        Modbus_WirteDI(3, 1);
        LedAlarmOn;
    }
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_13) == 0)
    {
        Modbus_WirteDI(3, 0);
        LedAlarmOff;
    }
}
/*!
    \brief      BSPInit(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void BSPInit(void)
{
    RCC_Configuration();
    NVIC_Configuration();
    TIM3Init();
    Vtimer_Init();
    QueueInit();
    Usart1Init();
    LPUartInit();
    Usart2Init();
    LedInit();
    RelayInit();
    LTE_ResetGpioInit();
    EnableDog();
    Key_gpioInit();
    ModeInput_gpioInit();
    BL0942_PWRGpioInit();
    Rtc_Init();
}

/*!
    \brief      BSPTask(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void BSPTask(void)
{
    Dog();
    LedTask();
    BL0942Task();
    Rtc_Task();
    ModeInput_Check();
}

/*!
    \brief      Delay_ms(uint32_t count) 毫秒延时函数
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Delay_ms(uint32_t count)
{
    int i, j;
    for (j = 0; j < count; j++)
        for (i = 0; i <= 3200; i++)
            ;
}
void Delay_MS(u16 Count)
{
    u16 i;
    for (i = 0; i < Count; i++)
    {
        system_delay_n_10us(50); // 实际上是20us
    }
}
void Delay_100uS(u16 Count)
{
    u16 i;
    for (i = 0; i < Count; i++)
    {
        system_delay_n_10us(5); // 实际上是20us
    }
}
