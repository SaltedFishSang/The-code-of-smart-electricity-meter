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
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
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
#include "protection.h"

struct _Switch_Status
{
    u8 SwitchOnvalue;   // 开关值
    u8 SwitchLockvalue; // 本地开关锁定
    u16 OverVValue;     // 过压值
    u16 UnderVValue;    // 欠压值
    u32 OverCValue;     // 过流值
    u32 OverPValue;     // 过载值
    u8 LoseCValue;      // 漏电值
    u8 OverTValue;      // 过温值
	  u8 SwitchTimevalue;//定时开关值
};
extern struct _Switch_Status Switch_Status;

#define _Printf_DBG 0
#define LEDNET_ON GPIO_ResetBits(GPIOA, GPIO_PIN_7)
#define LEDNET_OFF GPIO_SetBits(GPIOA, GPIO_PIN_7)
#define LEDLOCK_ON GPIO_ResetBits(GPIOC, GPIO_PIN_4)
#define LEDLOCK_OFF GPIO_SetBits(GPIOC, GPIO_PIN_4)
#define LEDSwitch_ON GPIO_ResetBits(GPIOB, GPIO_PIN_0)
#define LEDSwitch_OFF GPIO_SetBits(GPIOB, GPIO_PIN_0)

extern u8 g_Equipment_number[37];
extern uint8_t MqttRecBuf[500];
extern uint16_t MqttReclen;
extern void init_system(void);
extern void TIM_Configuration(void);
extern void BspInit(void);
extern void BspTask(void);
extern void BeepButton(u8 onoff);
extern void LedNetButton(u8 onoff);
extern void LedAlarmButton(u8 onoff);
extern void LedSwitchButton(u8 onoff);
extern void LedLockButton(u8 onoff);
extern u8 LosePwrCheck(void);
#endif
