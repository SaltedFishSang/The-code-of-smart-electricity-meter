#include "BSP.h"

_Uart Uart2 = {0};
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
}
/*!
    \brief     init usar5 串口5初始化
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
    \brief     init usar3 串口2初始化  BL0942使用
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
    USART_InitStructure.Mode = USART_MODE_TX|USART_MODE_RX;
    // init uart
    USART_Init(USART2, &USART_InitStructure);
    USART_ConfigInt(USART2, USART_INT_RXDNE, ENABLE);
		
    // enable uart
    USART_Enable(USART2, ENABLE);
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
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXDE) == RESET);
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
                  printf("chexksum ok\r\n");
                BL0942GetEleData(Uart2.RxBuf);
                  printf("checksum == 0x%x\r\n", checksum1);
                 printf("0x%x\r\n", Uart2.RxBuf[Uart2.RxLen - 1]);
            }
        }
        Uart2ClearRxBuf();
    }
}
