#ifndef _MQTT_H
#define _MQTT_H
#include "BSP.h"
extern void HeartInfo(void);
extern void HeartBeatflag(void);
extern void Uart0DataHandle(void);
extern void HeartInfoLimit(void);
extern void HeartInfoBegin(void);
extern void MqttrecedataHandle(u8 *data, u16 len, u8 flag);
extern u8 get_mqttlink(void);
extern void HeartStatus(void);
extern void set_mqttlink(u8 state);
#endif
