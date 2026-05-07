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
extern _Uart Uart2, Uart3, Uart4, Uart5;
extern void NVIC_Configuration(void);
extern void init_usart5(void);
extern void init_usart2(void);
extern void init_usart3(void);
extern void Uart2Task(void);
extern void Uart2Send(u8 *data, u16 len);
extern void Uart3Task(void);
extern void init_usart4(void);
extern void Uart4Send(u8 *data, u16 len);
extern uint8_t Uart5_UartStaus(void);
extern void Uart5ClearRxBuf(void);
extern void Uart5CopyRxData(uint8_t *data, uint8_t *len);
extern void Uart5Send(u8 *data, u16 len);
#endif
