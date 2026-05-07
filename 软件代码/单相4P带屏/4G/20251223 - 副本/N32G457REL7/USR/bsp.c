#include "bsp.h"
u8 g_Equipment_number[17] = "1210022509200001";
/*!
    \brief     init_system 系统时钟初始化
    \param[in]  none
    \param[out] none
    \retval     none
*/
void init_system(void)
{
    RCC_ClocksType clks;
    RCC_GetClocksFreqValue(&clks);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO | RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_GPIOC | RCC_APB2_PERIPH_GPIOD | RCC_APB2_PERIPH_GPIOE | RCC_APB2_PERIPH_GPIOF | RCC_APB2_PERIPH_GPIOG,
                            ENABLE);
    // RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_ETHMAC, ENABLE);
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART2 | RCC_APB1_PERIPH_USART3 | RCC_APB1_PERIPH_UART4, ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_Config(clks.HclkFreq / 100);
    NVIC_SetPriority(SysTick_IRQn, 0);
}
/*!
    \brief     TIM_Configuration  定时器TIM6初始化 配置给Vtimer使用
    \param[in]  none
    \param[out] none
    \retval     none
*/
TIM_TimeBaseInitType TIM_TimeBaseStructure;
OCInitType TIM_OCInitStructure;
uint16_t PrescalerValue = 0;
void TIM_Configuration(void)
{
    /* Compute the prescaler value */
    PrescalerValue = 7999; //(uint16_t) (SystemCoreClock / 12000000) - 1;
    /* PCLK1 = HCLK/4 */
    RCC_ConfigPclk1(RCC_HCLK_DIV4);
    /* TIM6 clock enable */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM6, ENABLE);
    NVIC_InitType NVIC_InitStructure;
    /* Enable the TIM6 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Time base configuration */
    TIM_TimeBaseStructure.Period = 3;
    TIM_TimeBaseStructure.Prescaler = 4;
    TIM_TimeBaseStructure.ClkDiv = 0;
    TIM_TimeBaseStructure.CntMode = TIM_CNT_MODE_UP;
    TIM_InitTimeBase(TIM6, &TIM_TimeBaseStructure);
    /* Prescaler configuration */
    TIM_ConfigPrescaler(TIM6, PrescalerValue, TIM_PSC_RELOAD_MODE_IMMEDIATE);
    /* TIM6 enable update irq */
    TIM_ConfigInt(TIM6, TIM_INT_UPDATE, ENABLE);
    /* TIM6 enable counter */
    TIM_Enable(TIM6, ENABLE);
}
/*!
    \brief     TIM6_IRQHandler  定时器Timer6中断 1ms一次
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIM6_IRQHandler(void)
{
    if (TIM_GetIntStatus(TIM6, TIM_INT_UPDATE) != RESET)
    {
        TIM_ClrIntPendingBit(TIM6, TIM_INT_UPDATE);
        Vtimer_UpdateHandler();
    }
}
/*!
    \brief     LedRun LED灯闪烁 间隔500ms
    \param[in]  none
    \param[out] none
    \retval     none
*/
static char ledflag = 0;
void LedRun(void)
{
    static u8 ledon = 0;
    if (ledflag == 1)
    {
        ledflag = 0;
        ledon++;
        if (ledon == 1)
        {
            GPIO_SetBits(GPIOA, GPIO_PIN_5); // 输出高电平
        }
        else
        {
            ledon = 0;
            GPIO_ResetBits(GPIOA, GPIO_PIN_5); // 输出高电平
        }
    }
}
/*!
    \brief     ledgpioInit LED GPIO初始化 配置成500ms闪烁
    \param[in]  none
    \param[out] none
    \retval     none
*/

void LedTime(void)
{
    ledflag = 1;
    Vtimer_SetTimer(VTIM1, 1000, LedTime);
}
void ledgpioInit(void)
{
    /*LED指示灯初始化 PA5*/
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_PIN_5);
    Vtimer_SetTimer(VTIM1, 500, LedTime);

    /*网络指示灯初始化 PA7*/
    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_PIN_7);

    /*告警指示灯初始化 PC4*/
    GPIO_InitStructure.Pin = GPIO_PIN_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_PIN_4);

    /*告警指示灯初始化 PB0*/
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_PIN_0);
}
/*!
    \brief     BeepButton 蜂鸣器开关
    \param[in]  onoff: 1:开 0:关
    \param[out] none
    \retval     none
*/
void LedNetButton(u8 onoff)
{
    if (onoff)
        LEDNET_ON;
    else
        LEDNET_OFF;
}
/*!
    \brief     LedAlarmButton 告警指示灯开关
    \param[in]  onoff: 1:开 0:关
    \param[out] none
    \retval     none
*/
void LedAlarmButton(u8 onoff)
{
    if (onoff)
        LEDAlarm_ON;
    else
        LEDAlarm_OFF;
}

/*!
    \brief     LedSwitchButton 输出指示灯开关
    \param[in]  onoff: 1:开 0:关
    \param[out] none
    \retval     none
*/
void LedSwitchButton(u8 onoff)
{
    if (onoff)
        LEDSwitch_ON;
    else
        LEDSwitch_OFF;
}
void Dog_Feed(void);
void EnableDog(void)
{
    /*看门狗喂狗引脚初始化 PC0*/
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);
    Vtimer_SetTimer(VTIM2, 30, Dog_Feed);
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
            GPIO_SetBits(GPIOC, GPIO_PIN_0); // 输出高电平
        }
        else
        {
            GPIO_ResetBits(GPIOC, GPIO_PIN_0); // 输出高电平
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
    Vtimer_SetTimer(VTIM2, 30, Dog_Feed);
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
    \brief     BeepgpioInit 蜂鸣器 GPIO初始化
    \param[in]  none
    \param[out] none
    \retval     none
*/
#define BEEP_ON GPIO_SetBits(GPIOA, GPIO_PIN_12)
#define BEEP_OFF GPIO_ResetBits(GPIOA, GPIO_PIN_12)
void BeepgpioInit(void)
{
    /* PA12*/
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_PIN_12);
}
/*!
    \brief     BeepButton 蜂鸣器开关
    \param[in]  onoff: 1:开 0:关
    \param[out] none
    \retval     none
*/
void BeepButton(u8 onoff)
{
    if (onoff)
        BEEP_ON;
    else
        BEEP_OFF;
}

/*!
    \brief     InputgpioInit 输入GPIO初始化 ：按键、停电检测
    \param[in]  none
    \param[out] none
    \retval     none
*/
void InputgpioInit(void)
{
    /*停电检测 PA0*/
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
}
/*!
    \brief     LosePwrCheck(void) 停电检测 返回1表示停电
    \param[in]  none
    \param[out] u8 1:有电 0:停电
    \retval     none
*/
u8 LosePwrCheck(void)
{
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_PIN_0) == RESET)
        return 1; // 有电
    else
        return 0; // 停电
}

void McuDrvUsart2GPIO_Config(void)
{
    GPIO_InitType GPIO_InitTypeStruct;

    GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitTypeStruct.Pin = GPIO_PIN_2;

    GPIO_InitPeripheral(GPIOA, &GPIO_InitTypeStruct);

    GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitTypeStruct.Pin = GPIO_PIN_3;

    GPIO_InitPeripheral(GPIOA, &GPIO_InitTypeStruct);
}
void BspInit(void)
{
    Vtimer_Init(); // 定时器初始化
   // EnableDog();   // 看门狗初始化
    QueueInit();   // 创建循环队列
    init_usart5(); // 串口5初始化
    init_usart2(); // 串口2 初始化
    init_usart4(); // 串口4 初始化
    NVIC_Configuration();
    BL0942_PWRGpioInit(); // BL0942初始化
    ledgpioInit();        // LED初始化
    BeepgpioInit();       // 蜂鸣器初始化
    InputgpioInit();      // 输入GPIO初始化
    TFTLCD_Init();        // TFT初始化
    RelaygpioInit();      // 继电器GPIO口初始化
    RS485Init();          // RS485初始化
    AT24CXX_Init();       // BL24C16初始化
    LTE_ResetGpioInit();  // Nbiot 复位引脚初始化
    keygpioInit();        // 按键输入引脚初始化
}

void BspTask(void)
{
    Dog();
    LedRun();
    BL0942Task();
    LTE_process();
}
