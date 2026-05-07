#ifndef _NBIOT_H
#define _NBIOT_H
#include "BSP.h"
#define NBBUFLEN 1500
extern u8 g_NBbuf[NBBUFLEN];
extern u16 g_NBRxCnt;
extern u8 g_csq;
extern u8 g_imei[15];
extern u8 g_qccid[20];
extern u8 g_csq;
extern u8 g_gettimeflag;
typedef struct
{
    u8 csq[5];
    u8 imei[16];
    u8 qccid[21];
    u8 gettimeflag;
} _LTE;
extern _LTE LTE;
extern void LTE_ResetGpioInit(void);
extern void LTE_process(void);
u8 NBUDP(void);
u8 get_NBWorkStatus(void);
u16 get_NBRxData(u8 *p);
void NB5310_process(void);
void set_NBReboot(void);
u8 set_NBTxData(u8 *p, u8 len);
extern void Set_LTE_SendData(u8 sendflag, char *topic, u16 topiclen, char *data, u16 datalen);
extern u8 QueryLteState(void);
extern u8 GetLTESendstatus(void);
extern void LTE_REBOOT(void);
#endif
