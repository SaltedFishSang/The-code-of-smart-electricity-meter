#ifndef _MQTT_APP_H
#define _MQTT_APP_H
#include "bsp.h"
extern mqtt_client_t static_client;
extern void MQTT_Client(void);
extern void HeartBeat(void);
extern void publish_task(mqtt_client_t *static_client);
extern void MQTT_lose(void);
extern void HeartBeatflag(void);
extern void Uart0DataHandle(void);
extern void HeartLimit(void);
extern void HeartBegin(void);
extern void Uart1DataHandle(void);
#endif
