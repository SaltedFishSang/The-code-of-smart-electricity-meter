#ifndef _app_net_H
#define _app_net_H
#include "bsp.h"
typedef struct 
{
	u8_t MACAddr[6];//MACµÿ÷∑
	u8_t IPAddr[4];
	u8_t GWIPAddr[4];
	u8_t IPMask[4];
	u32_t PORT;
	u8_t DESIP[4];
	u32_t DESPORT;
}Etnernet;
extern Etnernet EtnernetPara;
extern char *Iptostr(unsigned char *iptable);
extern char *Mactostr(unsigned char *iptable);
extern char *int_to_str(u32_t data);
extern void strtoiptable(char *str,unsigned char *table);
extern void net_init(void);
#endif
