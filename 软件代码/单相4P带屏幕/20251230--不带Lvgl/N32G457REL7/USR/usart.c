#include "BSP.h"

_Uart Uart2, Uart3 = {0};
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

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
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
    USART_InitStructure.Mode = USART_MODE_TX;
    // init uart
    USART_Init(UART5, &USART_InitStructure);
    USART_ConfigInt(UART5, USART_INT_RXDNE, ENABLE);
    // enable uart
    USART_Enable(UART5, ENABLE);
    data = UART5->STS;
}
/*!
    \brief     init usar2 ´®¿Ú2³õÊ¼»¯  BL0942Ê¹ÓÃ
    \param[in]  none
    \param[out] none
    \retval     none
*/
void init_usart2(void)
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_2;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.BaudRate = 9600;
    USART_InitStructure.WordLength = USART_WL_8B;
    USART_InitStructure.StopBits = USART_STPB_1;
    USART_InitStructure.Parity = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode = USART_MODE_TX | USART_MODE_RX;
    // init uart
    USART_Init(USART2, &USART_InitStructure);
    USART_ConfigInt(USART2, USART_INT_RXDNE, ENABLE);

    // enable uart
    USART_Enable(USART2, ENABLE);
}

/*!
    \brief     init usar3 ´®¿Ú3³õÊ¼»¯  RS485Ê¹ÓÃ
    \param[in]  none
    \param[out] none
    \retval     none
*/
void init_usart3(void)
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.BaudRate = 9600;
    USART_InitStructure.WordLength = USART_WL_8B;
    USART_InitStructure.StopBits = USART_STPB_1;
    USART_InitStructure.Parity = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode = USART_MODE_TX | USART_MODE_RX;
    // init uart
    USART_Init(USART3, &USART_InitStructure);
    USART_ConfigInt(USART3, USART_INT_RXDNE, ENABLE); 
    // enable uart
    USART_Enable(USART3, ENABLE);
}


/*!
    \brief     init usar4 ´®¿4³õÊ¼»¯  RS485Ê¹ÓÃ
    \param[in]  none
    \param[out] none
    \retval     none
*/
void init_usart4(void)
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);

    USART_InitStructure.BaudRate = 115200;
    USART_InitStructure.WordLength = USART_WL_8B;
    USART_InitStructure.StopBits = USART_STPB_1;
    USART_InitStructure.Parity = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode = USART_MODE_TX | USART_MODE_RX;
    // init uart
    USART_Init(UART4, &USART_InitStructure);
    USART_ConfigInt(UART4, USART_INT_RXDNE, ENABLE); 
    // enable uart
    USART_Enable(UART4, ENABLE);
}
/*!
    \brief     Usart2_RxOverTime
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Usart2_RxOverTime(void)
{
    Uart2.RxFlag = 1;
    Vtimer_KillTimer(VTIM3);
}

void USART2_IRQHandler(void)
{
    u8 rec;
    if (USART_GetIntStatus(USART2, USART_INT_RXDNE) != RESET)
    {
        if (Uart2.RxFlag == 0)
        {
            rec = USART_ReceiveData(USART2);
            Uart2.RxBuf[Uart2.RxLen] = rec;
            Uart2.RxLen++;
            Vtimer_KillTimer(VTIM3);
            Vtimer_SetTimer(VTIM3, 50, Usart2_RxOverTime);
        }
        else
            rec = USART_ReceiveData(USART2);
    }
    else
        rec = USART_ReceiveData(USART2);
}

/*!
    \brief    Uart2_UartStaus
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t Uart2_UartStaus(void)
{
    if (Uart2.RxFlag == 1)
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
void Uart2ClearRxBuf()
{
    uint8_t i;
    for (i = 0; i < Uart2.RxLen; i++)
        Uart2.RxBuf[i] = 0;
    Uart2.RxLen = 0;
    Uart2.RxFlag = 0;
}

/*!
    \brief    Uart2CopyRxData
    \param[in]   uint8_t *
    \param[out]  uint8_t *
    \retval     none
*/
void Uart2CopyRxData(uint8_t *data, uint8_t *len)
{
    uint8_t i;
    for (i = 0; i < Uart2.RxLen; i++)
        data[i] = Uart2.RxBuf[i];
    *len = Uart2.RxLen;
    Uart2ClearRxBuf();
}

/*!
    \brief    Uart2Send
    \param[in]   uint8_t *
    \param[out]  uint8_t *
    \retval     none
*/
void Uart2Send(u8 *data, u16 len)
{
    u16 i;
    for (i = 0; i < len; i++)
    {
        USART_SendData(USART2, data[i]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXDE) == RESET)
            ;
    }
}
/*!
    \brief      Uart2Task(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Uart2Task(void)
{
    u16 checksum = 0;
    u16 checksum1 = 0;
    u8 i = 0;
    checksum = 0x58;
    if (Uart2_UartStaus())
    {
        if (23 == Uart2.RxLen)
        {
            for (i = 0; i < Uart2.RxLen - 1; i++)
                checksum += Uart2.RxBuf[i];
            checksum1 = ((~(checksum & 0x00FF)) & 0x00FF);
            if (Uart2.RxBuf[Uart2.RxLen - 1] == checksum1)
            {
                // printf("chexksum ok\r\n");
                BL0942GetEleData(Uart2.RxBuf);
                // printf("checksum == 0x%x\r\n", checksum1);
                // printf("0x%x\r\n", Uart2.RxBuf[Uart2.RxLen - 1]);
            }
        }
        Uart2ClearRxBuf();
    }
}

/*!
    \brief     Usart3_RxOverTime
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Usart3_RxOverTime(void)
{
    Uart3.RxFlag = 1;
    Vtimer_KillTimer(VTIM5);
}

void USART3_IRQHandler(void)
{
    u8 rec;
    if (USART_GetIntStatus(USART3, USART_INT_RXDNE) != RESET)
    {
        if (Uart3.RxFlag == 0)
        {
            rec = USART_ReceiveData(USART3);
            Uart3.RxBuf[Uart3.RxLen] = rec;
            Uart3.RxLen++;
            Vtimer_KillTimer(VTIM5);
            Vtimer_SetTimer(VTIM5, 50, Usart3_RxOverTime);
        }
        else
            rec = USART_ReceiveData(USART3);
    }
    else
        rec = USART_ReceiveData(USART3);
}

/*!
    \brief    Uart3_UartStaus
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t Uart3_UartStaus(void)
{
    if (Uart3.RxFlag == 1)
        return 1;
    else
        return 0;
}

/*!
    \brief    Uart3ClearRxBuf
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Uart3ClearRxBuf()
{
    uint8_t i;
    for (i = 0; i < Uart3.RxLen; i++)
        Uart3.RxBuf[i] = 0;
    Uart3.RxLen = 0;
    Uart3.RxFlag = 0;
}

/*!
    \brief    Uart3CopyRxData
    \param[in]   uint8_t *
    \param[out]  uint8_t *
    \retval     none
*/
void Uart3CopyRxData(uint8_t *data, uint8_t *len)
{
    uint8_t i;
    for (i = 0; i < Uart3.RxLen; i++)
        data[i] = Uart3.RxBuf[i];
    *len = Uart3.RxLen;
    Uart3ClearRxBuf();
}

/*!
    \brief    Uart3Send
    \param[in]   uint8_t *
    \param[out]  uint8_t *
    \retval     none
*/
void Uart3Send(u8 *data, u16 len)
{
    u16 i;
    for (i = 0; i < len; i++)
    {
        USART_SendData(USART3, data[i]);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXDE) == RESET)
            ;
    }
}
/*!
    \brief      Uart3Task(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Uart3Task(void)
{
    if (Uart3_UartStaus())
    {
			  RS485_DIR_TX;
			  Uart3Send(Uart3.RxBuf,Uart3.RxLen);
			  delay_ms(100);
			  RS485_DIR_RX;
        Uart3ClearRxBuf();
    }
}





void UART4_IRQHandler(void)
{
    u8 rec;
		       rec = USART_ReceiveData(UART4);
          In_Queue(MyQueue, rec);
}

/*!
    \brief    Uart3Send
    \param[in]   uint8_t *
    \param[out]  uint8_t *
    \retval     none
*/
void Uart4Send(u8 *data, u16 len)
{
    u16 i;
    for (i = 0; i < len; i++)
    {
        USART_SendData(UART4, data[i]);
        while (USART_GetFlagStatus(UART4, USART_FLAG_TXDE) == RESET)
            ;
    }
}
