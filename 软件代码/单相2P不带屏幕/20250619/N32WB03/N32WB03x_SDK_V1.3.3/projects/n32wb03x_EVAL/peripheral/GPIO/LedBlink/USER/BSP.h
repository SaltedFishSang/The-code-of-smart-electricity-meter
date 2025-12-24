#ifndef _BSP_H
#define _BSP_H
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned char u8;
extern u8 g_Equipment_number[17];
#include "main.h"
#include "n32wb03x.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "n32wb03x_exti.h"
#include "n32wb03x_lpuart.h"
#include "system_n32wb03x.h"
#include "n32wb03x_qflash.h"
#include "n32wb03x_rtc.h"
#include "Usart.h"
#include "V_Timer.h"
#include "Relay.h"
#include "n32wb03x.h"
#include "BL0942.h"
#include "Nbiot.h"
#include "CircularQueue.h"
#include "typechange.h"
#include "Modbus.h"
#include "RTC.h"
#include "Flash.h"
#define log_info(x, ...) printf("%s:%s:%d:" x "", \
                                __FILE__,         \
                                __FUNCTION__,     \
                                __LINE__,         \
                                ##__VA_ARGS__)
#define LedWorkOn GPIO_ResetBits(GPIOA, GPIO_PIN_6)
#define LedWorkOff GPIO_SetBits(GPIOA, GPIO_PIN_6)

#define LedNetOn GPIO_ResetBits(GPIOB, GPIO_PIN_11)
#define LedNetOff GPIO_SetBits(GPIOB, GPIO_PIN_11)

extern void LedInit(void);

extern void RCC_Configuration(void);

extern void NVIC_Configuration(void);

extern void TIM3Init(void);

extern void BSPInit(void);

extern void BSPTask(void);

extern void Delay_ms(uint32_t count);

void Delay_MS(u16 Count);
void Delay_100uS(u16 Count);
#endif
