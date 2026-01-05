#ifndef _bsp_H
#define _bsp_H

#include "n32g45x.h"
#include "n32g45x_rcc.h"
#include "n32g45x_gpio.h"
#include "n32g45x_exti.h"
#include "n32g45x_rtc.h"
#include "n32g45x_bkp.h"
#include "n32g45x_pwr.h"
#include "misc.h"
#include "log.h"
#include "V_Timer.h"
#include "usart.h"
#include "stdlib.h"
#include "typechange.h"
#include "TFT.h"
#include "delay.h"
#include "Relay.h"
#include "BSP.h"
#include "BL0942.h"
#include "string.h"
#include "BSP.h"
#include "RS485.h"
#include "EPROM.h"
#include "CircularQueue.h"
#include "Nbiot.h"
#include "typechange.h"
#include "Key.h"
#include "LCD_Display.h"
#include "RTC.h"
#include "n32g45x_exti.h"
#include "Modbus.h"
#include "MQTT.h"
#define _Printf_DBG 0
#define LEDNET_ON GPIO_ResetBits(GPIOA, GPIO_PIN_7)
#define LEDNET_OFF GPIO_SetBits(GPIOA, GPIO_PIN_7)
#define LEDLOCK_ON GPIO_ResetBits(GPIOC, GPIO_PIN_4)
#define LEDLOCK_OFF GPIO_SetBits(GPIOC, GPIO_PIN_4)
#define LEDSwitch_ON GPIO_ResetBits(GPIOB, GPIO_PIN_0)
#define LEDSwitch_OFF GPIO_SetBits(GPIOB, GPIO_PIN_0)

extern u8 g_Equipment_number[17];
extern void init_system(void);
extern void TIM_Configuration(void);
extern void BspInit(void);
extern void BspTask(void);
extern void BeepButton(u8 onoff);
extern void LedNetButton(u8 onoff);
extern void LedAlarmButton(u8 onoff);
extern void LedSwitchButton(u8 onoff);
extern void LedLockButton(u8 onoff);
#endif
