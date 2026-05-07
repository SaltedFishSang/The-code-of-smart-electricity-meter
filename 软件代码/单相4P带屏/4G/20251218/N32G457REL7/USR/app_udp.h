#ifndef _app_udp_H
#define _app_udp_H
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/ip.h"
#include "lwip/icmp.h"
#include "lwip/err.h"
extern struct udp_pcb *UdpPCB;
extern void UdpDataSend(unsigned short len);
extern void udp_echoclient_connect(void);
extern void udpsend(u8_t *bySendBuf, u16_t wSendLent);
#endif

