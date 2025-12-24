#ifndef _MQTT_H
#define _MQTT_H
#include "BSP.h"
extern uint8_t MqttRecBuf[500];
extern uint16_t MqttReclen;
extern u8 g_mqtt_errnum;
extern void HeartInfo(void);
extern void HeartBeatflag(void);
extern void Uart0DataHandle(void);
extern void HeartInfoLimit(void);
extern void HeartInfoBegin(void);
extern void MqttrecedataHandle(u8 *data, u16 len, u8 flag);
extern void HeartStausBegin(void);
extern u8 get_mqttlink(void);
extern void HeartStatus(void);
#endif
