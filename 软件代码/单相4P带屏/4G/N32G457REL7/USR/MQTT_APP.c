#include "bsp.h"
/* dns variables & functions */
#define MQTT_HOSTNAME "iot.eclipse.org"
u8 Heartflag = 0;
void MQTT_Client(void);
void HeartBegin(void)
{
    Vtimer_SetTimer(VTIM3, 100000, HeartBeatflag);
}
void HeartBeatflag(void)
{
    Heartflag = 1;
    Vtimer_SetTimer(VTIM3, 100000, HeartBeatflag);
}
/*!
    \brief      HeartBeat
    \param[in]  client: MQTT client
    \param[out] none
    \retval     none
*/

void HeartBeat(void)
{
    char p[50], p1[100];
    if (Heartflag)
    {
			
			 // printf("\r\nm_err_cnt = %d\r\n", m_err_cnt++);
        Heartflag = 0;
        sprintf(p, "modbus_info/%s", g_Equipment_number);
        sprintf(p1, "%d.%d.%d.%d_%02x-%02x-%02x-%02x-%02x-%02x_0",
                EtnernetPara.IPAddr[0], EtnernetPara.IPAddr[1], EtnernetPara.IPAddr[2], EtnernetPara.IPAddr[3], EtnernetPara.MACAddr[0],
                EtnernetPara.MACAddr[1], EtnernetPara.MACAddr[2], EtnernetPara.MACAddr[3], EtnernetPara.MACAddr[4], EtnernetPara.MACAddr[5]);
        //printf("%s\r\n", p1);
        new_meg_publish(client11, NULL, p, p1, 0);
        g_mqtt_errnum++;
        if (g_mqtt_errnum >= 3)
        {
					g_mqtt_errnum = 0;
            MQTT_Client();
            NETLED_OFF;
            NVIC_SystemReset();
        }
    }
}
void HeartLimit(void)
{
    char p[50], p1[100];
    sprintf(p, "modbus_info/%s",
            g_Equipment_number);
    sprintf(p1, "%d.%d.%d.%d_%02x-%02x-%02x-%02x-%02x-%02x_0",
            EtnernetPara.IPAddr[0], EtnernetPara.IPAddr[1], EtnernetPara.IPAddr[2], EtnernetPara.IPAddr[3], EtnernetPara.MACAddr[0],
            EtnernetPara.MACAddr[1], EtnernetPara.MACAddr[2], EtnernetPara.MACAddr[3], EtnernetPara.MACAddr[4], EtnernetPara.MACAddr[5]);
    new_meg_publish(client11, NULL, p, p1, 0);
}
void MQTT_Client(void)
{
    MQTT_lose();
    client11 = mqtt_client_new();
    IP4_ADDR(&domain_ipaddr, EtnernetPara.DESIP[0], EtnernetPara.DESIP[1], EtnernetPara.DESIP[2], EtnernetPara.DESIP[3]);
    if (client11 != NULL)
    {
#if _Printf_DBG
        printf("creat mqtt client  begin conncting\r\n");
#endif
        example_do_connect(client11);
    }
}

/*!
     \brief      MQTT_lose:   Once MQTT fails, release and disconnect the MQTT client
    \param[in]  client: MQTT client
    \param[out] none
    \retval     none
*/
void MQTT_lose(void)
{
    if (client11 != NULL)
    {
        mqtt_client_free(client11);
        mqtt_disconnect(client11);
    }
}

 void Uart1DataHandle(void)
{
    if (Uart1_UartStaus())
    {
        /*恢复出厂设置*/
        if ((Uart1.RxBuf[0] == 'A') && (Uart1.RxBuf[1] == 'T') && (Uart1.RxBuf[2] == '+') && (Uart1.RxBuf[3] == 'R') && (Uart1.RxBuf[4] == 'e') && (Uart1.RxBuf[5] == 'b'))
        {
            FlasgWriteResetData();
            NVIC_SystemReset();
        }
        else
        {
            /*恢复出厂设置*/
            if ((Uart1.RxBuf[0] == 'A') && (Uart1.RxBuf[1] == 'T') && (Uart1.RxBuf[2] == '+') && (Uart1.RxBuf[3] == 'R') && (Uart1.RxBuf[4] == 'e') && (Uart1.RxBuf[5] == 's'))
            {
                NVIC_SystemReset();
            }
            else if(1)
            {
                /*设置ID*/
                if ((Uart1.RxBuf[0] == 'A') && (Uart1.RxBuf[1] == 'T') && (Uart1.RxBuf[2] == '+') && (Uart1.RxBuf[3] == 'I') && (Uart1.RxBuf[4] == 'D'))
                {
                    memcpy(g_Equipment_number, &Uart1.RxBuf[5], 16);
                    g_Equipment_number[16] = '\0';
                    // printf("set id\r\n");
                    FlashWrite_mac_id();
                    NVIC_SystemReset();
                }
                else if(1)
                {
                    if ((Uart1.RxBuf[0] == 'A') && (Uart1.RxBuf[1] == 'T') && (Uart1.RxBuf[2] == '+') && (Uart1.RxBuf[3] == 'M') && (Uart1.RxBuf[4] == 'A') && (Uart1.RxBuf[5] == 'C'))
                    {
                        Ascii2Hex(EtnernetPara.MACAddr, &Uart1.RxBuf[6], 12);
                        // printf("Mac_addr==0x%02x.0x%02x.0x%02x.0x%02x.0x%02x.0x%02x\r\n", EtnernetPara.MACAddr[0], EtnernetPara.MACAddr[1], EtnernetPara.MACAddr[2], EtnernetPara.MACAddr[3], EtnernetPara.MACAddr[4], EtnernetPara.MACAddr[5]);
                        // printf("set mac\r\n");
                        FlashWrite_mac_id();
                        NVIC_SystemReset();
                    }
                    else 
                    {
												char p[300] = {0};
												sprintf(p, "modbus_status/%s",
																g_Equipment_number);
												if(Uart1.RxLen > 5)
                           mqtt_publish(client11, p, Uart1.RxBuf, Uart1.RxLen, 1, 0, mqtt_pub_request_cb, NULL);
                        Uart1ClearRxBuf();
                    }
                }  
            }
        }
        Uart1ClearRxBuf();
    }
}
