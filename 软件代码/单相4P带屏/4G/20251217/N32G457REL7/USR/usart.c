#include "BSP.h"

_Uart Uart1 = {0};
/*!
    \brief      NVIC_Configuration
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*!
    \brief     init usar5 ´®¿Ú5³õÊ¼»¯
    \param[in]  none
    \param[out] none
    \retval     none
*/
void init_usart5(void)
{
    u8 data;
    (void)data;
    NVIC_Configuration();
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO | RCC_APB2_PERIPH_GPIOC | RCC_APB2_PERIPH_GPIOD, ENABLE);
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_UART5, ENABLE);
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.BaudRate = 115200;
    USART_InitStructure.WordLength = USART_WL_8B;
    USART_InitStructure.StopBits = USART_STPB_1;
    USART_InitStructure.Parity = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode = USART_MODE_TX ;
    // init uart
    USART_Init(UART5, &USART_InitStructure);
    USART_ConfigInt(UART5, USART_INT_RXDNE, ENABLE);
    // enable uart
    USART_Enable(UART5, ENABLE);
    data = UART5->STS;
}

/*!
    \brief     Usart2_RxOverTime
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Usart1_RxOverTime(void)
{
    Uart1.RxFlag = 1;
    Vtimer_KillTimer(VTIM4);
}

void USART1_IRQHandler(void)
{
    u8 rec;
    if (USART_GetIntStatus(USART1, USART_INT_RXDNE) != RESET)
    {
        if (Uart1.RxFlag == 0)
        {
            rec = USART_ReceiveData(USART1);
            Uart1.RxBuf[Uart1.RxLen] = rec;
            Uart1.RxLen++;
            Vtimer_KillTimer(VTIM4);
            Vtimer_SetTimer(VTIM4, 50, Usart1_RxOverTime);
        }
        else
            rec = USART_ReceiveData(USART1);
    }
    else
        rec = USART_ReceiveData(USART1);
}

/*!
    \brief    Uart2_UartStaus
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t Uart1_UartStaus(void)
{
    if (Uart1.RxFlag == 1)
        return 1;
    else
        return 0;
}

/*!
    \brief    Uart2ClearRxBuf
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Uart1ClearRxBuf()
{
    uint8_t i;
    for (i = 0; i < Uart1.RxLen; i++)
        Uart1.RxBuf[i] = 0;
    Uart1.RxLen = 0;
    Uart1.RxFlag = 0;
}

/*!
    \brief    Uart2CopyRxData
    \param[in]   uint8_t *
    \param[out]  uint8_t *
    \retval     none
*/
void Uart1CopyRxData(uint8_t *data, uint8_t *len)
{
    uint8_t i;
    for (i = 0; i < Uart1.RxLen; i++)
        data[i] = Uart1.RxBuf[i];
    *len = Uart1.RxLen;
    Uart1ClearRxBuf();
}

/*!
    \brief    Uart2Send
    \param[in]   uint8_t *
    \param[out]  uint8_t *
    \retval     none
*/
void Uart1Send(u8 *data, u16 len)
{
    u16 i;
    for (i = 0; i < len; i++)
    {
        USART_SendData(USART1, data[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXC) == RESET)
        {
        }
    }
}
