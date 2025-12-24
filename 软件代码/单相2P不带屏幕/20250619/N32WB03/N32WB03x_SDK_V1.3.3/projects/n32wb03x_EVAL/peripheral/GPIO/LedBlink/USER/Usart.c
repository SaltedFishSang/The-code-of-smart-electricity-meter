#include "BSP.h"
_Uart Uart2, Uart1;

int fputc(int ch, FILE *f)
{
    LPUART_SendData(LPUART1, (uint8_t)ch);
    while (LPUART_GetFlagStatus(LPUART1, LPUART_FLAG_TXC) == RESET)
        ;
    LPUART_ClrFlag(LPUART1, LPUART_FLAG_TXC);
    return (ch);
}
/*!
    \brief      Usart2gpioInit(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Usart2gpioInit(void)
{
    GPIO_InitType GPIO_InitStructure;
    /* Configure USARTy Tx as alternate function push-pull */
    GPIO_InitStructure.Pin = GPIO_PIN_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_USART2;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    /* Configure USARTx Rx as alternate function push-pull */
    GPIO_InitStructure.Pin = GPIO_PIN_5;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_USART2;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
}

/*!
    \brief      Usart2Init(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Usart2Init(void)
{
    USART_InitType USART_InitStructure;
    Usart2gpioInit();
    /* USARTy and USARTz configuration ------------------------------------------------------*/
    USART_InitStructure.BaudRate = 9600;
    USART_InitStructure.WordLength = USART_WL_8B;
    USART_InitStructure.StopBits = USART_STPB_1;
    USART_InitStructure.Parity = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode = USART_MODE_RX | USART_MODE_TX;
    /* Configure USARTy and USARTz */
    USART_Init(USART2, &USART_InitStructure);
    /* Enable USARTy Receive and Transmit interrupts */
    USART_ConfigInt(USART2, USART_INT_RXDNE, ENABLE);
    /* Enable the USARTy and USARTz */
    USART_Enable(USART2, ENABLE);
}

/*!
    \brief      串口2 接收超时等待
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Usart2_RxOverTime(void)
{
    Uart2.ReceiveFlag = 1;
    Vtimer_KillTimer(VTIM2);
}
/*!
    \brief      清除串口2数据接收缓存
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Uart2ClearRxBuf(void)
{
    uint16_t i;
    for (i = 0; i < Uart2.RxLen; i++)
        Uart2.RxBuf[i] = 0;
    Uart2.RxLen = 0;
    Uart2.ReceiveFlag = 0;
}
/*!
    \brief      判断串口2数据是否接收完成
    \param[in]  None
    \param[out] none
    \retval     none
*/
uint8_t Uart2_UartStaus(void)
{
    if (Uart2.ReceiveFlag == 1)
    {
        return 1;
    }
    else
        return 0;
}
/*!
    \brief      串口2发送字节函数
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Uart2_SendData(u8 *data, u16 datalen)
{
    u16 i = 0;
    for (i = 0; i < datalen; i++)
    {
        USART_SendData(USART2, (u16)data[i]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXDE) == RESET)
            ;
        // Delay_100uS(3);
    }
}
void USART2_IRQHandler(void)
{
    u8 rec;
    if (USART_GetIntStatus(USART2, USART_INT_RXDNE) != RESET)
    {
        if (Uart2.ReceiveFlag == 0)
        {
            rec = USART_ReceiveData(USART2);
            ;
            Uart2.RxBuf[Uart2.RxLen] = rec;
            Uart2.RxLen++;
            Vtimer_KillTimer(VTIM2);
            Vtimer_SetTimer(VTIM2, 50, Usart2_RxOverTime);
        }
        else
        {
            rec = USART_ReceiveData(USART2);
        }
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
                //  printf("chexksum ok\r\n");
                BL0942GetEleData(Uart2.RxBuf);
                //  printf("checksum == 0x%x\r\n", checksum1);
                // printf("0x%x\r\n", Uart2.RxBuf[Uart2.RxLen - 1]);
            }
        }
        Uart2ClearRxBuf();
    }
}

/*!
    \brief      LPUartgpioInit(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void LPUartgpioInit(void)
{
    GPIO_InitType GPIO_InitStructure;
    /* Configure USARTy Tx as alternate function push-pull */
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_LPUART1;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    /* Configure USARTx Rx as alternate function push-pull */
    GPIO_InitStructure.Pin = GPIO_PIN_11;
    GPIO_InitStructure.GPIO_Pull = GPIO_PULL_UP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_LPUART1;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
}

/*!
    \brief      LPUartInit(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void LPUartInit(void)
{
    LPUART_InitType LPUART_InitStructure;
    LPUartgpioInit();
    // LPUART_DeInit(LPUART1);
    /* LPUARTx configuration ------------------------------------------------------*/
    LPUART_InitStructure.BaudRate = 115200;
    LPUART_InitStructure.Parity = LPUART_PE_NO;
    LPUART_InitStructure.RtsThreshold = LPUART_RTSTH_FIFOFU;
    LPUART_InitStructure.HardwareFlowControl = LPUART_HFCTRL_NONE;
    LPUART_InitStructure.Mode = LPUART_MODE_RX | LPUART_MODE_TX;
    /* Configure LPUARTx */
    LPUART_Init(LPUART1, &LPUART_InitStructure);
}

/*!
    \brief      Usart1gpioInit(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Usart1gpioInit(void)
{
    GPIO_InitType GPIO_InitStructure;
    /* Configure USARTy Tx as alternate function push-pull */
    GPIO_InitStructure.Pin = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_USART1;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    /* Configure USARTx Rx as alternate function push-pull */
    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_USART1;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
}

/*!
    \brief      Usart2Init(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Usart1Init(void)
{
    USART_InitType USART_InitStructure;
    Usart1gpioInit();
    /* USARTy and USARTz configuration ------------------------------------------------------*/
    USART_InitStructure.BaudRate = 115200;
    USART_InitStructure.WordLength = USART_WL_8B;
    USART_InitStructure.StopBits = USART_STPB_1;
    USART_InitStructure.Parity = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode = USART_MODE_RX | USART_MODE_TX;
    /* Configure USARTy and USARTz */
    USART_Init(USART1, &USART_InitStructure);
    /* Enable USARTy Receive and Transmit interrupts */
    USART_ConfigInt(USART1, USART_INT_RXDNE, ENABLE);
    /* Enable the USARTy and USARTz */
    USART_Enable(USART1, ENABLE);
}
/*!
    \brief      串口2 接收超时等待
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Usart1_RxOverTime(void)
{
    Uart1.ReceiveFlag = 1;
    Vtimer_KillTimer(VTIM4);
}
/*!
    \brief      清除串口2数据接收缓存
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Uart1ClearRxBuf(void)
{
    uint16_t i;
    for (i = 0; i < Uart1.RxLen; i++)
        Uart1.RxBuf[i] = 0;
    Uart1.RxLen = 0;
    Uart1.ReceiveFlag = 0;
}
/*!
    \brief      判断串口2数据是否接收完成
    \param[in]  None
    \param[out] none
    \retval     none
*/
uint8_t Uart1_UartStaus(void)
{
    if (Uart1.ReceiveFlag == 1)
    {
        return 1;
    }
    else
        return 0;
}
/*!
    \brief      串口1发送字节函数
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Uart1_SendData(u8 *data, u16 datalen)
{
    u16 i = 0;
    for (i = 0; i < datalen; i++)
    {
        USART_SendData(USART1, (u16)data[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXDE) == RESET)
            ;
        // Delay_100uS(3);
    }
}

void USART1_IRQHandler(void)
{
    u8 rec;
    if (USART_GetIntStatus(USART1, USART_INT_RXDNE) != RESET)
    {
        rec = USART_ReceiveData(USART1);
        In_Queue(MyQueue, rec);
    }
}
