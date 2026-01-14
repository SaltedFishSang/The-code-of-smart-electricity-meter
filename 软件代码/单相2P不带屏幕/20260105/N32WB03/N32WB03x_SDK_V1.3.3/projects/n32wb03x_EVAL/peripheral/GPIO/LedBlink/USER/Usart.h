#ifndef _Usart_H
#define _Usart_H

#include "BSP.h"

typedef struct
{
    uint8_t RxBuf[1000];
    uint8_t TxBuf[1000];
    uint16_t RxLen;
    uint16_t Txnum;
    uint8_t ReceiveFlag;
} _Uart;
extern _Uart Uart2,Uart1;
extern void Usart2gpioInit(void);
extern void Usart2Init(void);
extern void Uart2_SendData(u8 *data, u16 datalen);
extern void Uart2Task(void);
extern void LPUartInit(void);
extern void Usart1Init(void);
extern void Uart1_SendData(u8 *data, u16 datalen);
extern void Uart1ClearRxBuf(void);
#endif
