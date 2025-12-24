#ifndef __usart_H
#define __usart_H
#include "BSP.h"
typedef struct
{
	u8 RxFlag;
	u16 RxLen;
	u8 RxBuf[1000];
	u8 TxBuf[100];
	u16 TxNum;
} _Uart;
extern _Uart Uart1;
extern void NVIC_Configuration(void);
extern void init_usart5(void);
void Usart1_RxOverTime(void);
extern uint8_t Uart1_UartStaus(void);
void Uart1ClearRxBuf(void);
extern void Uart1CopyRxData(u8 *data, u8 *len);
extern void Uart1Send(u8 *data, u16 len);
extern void Uart1_DataHandle(void);
#endif
