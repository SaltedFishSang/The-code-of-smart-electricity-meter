#ifndef _RS485_H
#define _RS485_H
#include "BSP.h"
#define RS485_DIR_RX GPIO_ResetBits(GPIOA, GPIO_PIN_11);
#define RS485_DIR_TX GPIO_SetBits(GPIOA, GPIO_PIN_11);
extern void RS485Init(void);
extern void RS485Task(void);
#endif
