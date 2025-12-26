#include "bsp.h"
/* dns variables & functions */
#define MQTT_HOSTNAME "iot.eclipse.org"
u8 Heartflag = 0;
mqtt_client_t static_client;

void HeartBegin(void)
{
	 Vtimer_SetTimer(VTIM3, 60000, HeartBeatflag);
}
void HeartBeatflag(void)
{
    Heartflag = 1;
    Vtimer_SetTimer(VTIM3, 60000, HeartBeatflag);
}
/*!
    \brief      HeartBeat
    \param[in]  client: MQTT client
    \param[out] none
    \retval     none
*/

	
void HeartBeat(void)
{
    if (Heartflag)
    {
        Heartflag = 0;
        char *p = (char *)malloc(50 * sizeof(char));
			  char *p1 = (char *)malloc(100 * sizeof(char));
        if (p != NULL)
        {
					sprintf(p, "modbus_info/%s",g_Equipment_number);
        }
				
				 if (p1 != NULL)
        {
					sprintf(p1, "%d.%d.%d.%d_%02x-%02x-%02x-%02x-%02x-%02x_0",
                    EtnernetPara.IPAddr[0],EtnernetPara.IPAddr[1],EtnernetPara.IPAddr[2],EtnernetPara.IPAddr[3],EtnernetPara.MACAddr[0],\
					          EtnernetPara.MACAddr[1],EtnernetPara.MACAddr[2],EtnernetPara.MACAddr[3],EtnernetPara.MACAddr[4],EtnernetPara.MACAddr[5]);
        }
				printf("%s\r\n",p1);
        new_meg_publish(&static_client, NULL, p, p1, 0);
        free(p);
				free(p1);
				g_mqtt_errnum++;
        if (g_mqtt_errnum >= 3)
        {
            g_mqtt_errnum = 0;
            g_mqtt_errnumsum++;
            MQTT_lose();
            MQTT_Client();
            NETLED_OFF;
        }
        if (g_mqtt_errnumsum >= 1)
        {
            NVIC_SystemReset();
        }
    }
}
void HeartLimit(void)
{
	      char *p = (char *)malloc(50 * sizeof(char));
			  char *p1 = (char *)malloc(100 * sizeof(char));
        if (p != NULL)
        {
					sprintf(p, "modbus_info/%s",
                    g_Equipment_number);
        }
				
				 if (p1 != NULL)
        {
					sprintf(p1,"%d.%d.%d.%d_%02x-%02x-%02x-%02x-%02x-%02x_0",
                    EtnernetPara.IPAddr[0],EtnernetPara.IPAddr[1],EtnernetPara.IPAddr[2],EtnernetPara.IPAddr[3],EtnernetPara.MACAddr[0],\
					          EtnernetPara.MACAddr[1],EtnernetPara.MACAddr[2],EtnernetPara.MACAddr[3],EtnernetPara.MACAddr[4],EtnernetPara.MACAddr[5]);
        }
				//	printf("%s\r\n",p1);
        new_meg_publish(&static_client, NULL, p, p1, 0);
        free(p);
				free(p1);
}
void MQTT_Client(void)
{
    IP4_ADDR(&domain_ipaddr, EtnernetPara.DESIP[0], EtnernetPara.DESIP[1], EtnernetPara.DESIP[2], EtnernetPara.DESIP[3]);
    /* connect to mqtt server */
#if _Printf_DBG
    printf("creat mqtt client  begin conncting\r\n");
#endif
    static_client.conn_state = 0;
    example_do_connect(&static_client);
}

/*!
     \brief      MQTT_lose:   Once MQTT fails, release and disconnect the MQTT client
    \param[in]  client: MQTT client
    \param[out] none
    \retval     none
*/
void MQTT_lose(void)
{
    mqtt_client_free(&static_client);
    mqtt_disconnect(&static_client);
}

void Uart1DataHandle(void)
{
    if (Uart1_UartStaus())
    {
        /*恢复出厂设置*/
        if ((Uart1.RxBuf[0] == 'A') && (Uart1.RxBuf[1] == 'T') && (Uart1.RxBuf[2] == '+') && (Uart1.RxBuf[3] == 'R') && (Uart1.RxBuf[4] == 'e') && (Uart1.RxBuf[5] == 'b'))
        {
           // FlasgWriteResetData();
            NVIC_SystemReset();
        }
        else
        {
            /*恢复出厂设置*/
            if ((Uart1.RxBuf[0] == 'A') && (Uart1.RxBuf[1] == 'T') && (Uart1.RxBuf[2] == '+') && (Uart1.RxBuf[3] == 'R') && (Uart1.RxBuf[4] == 'e') && (Uart1.RxBuf[5] == 's'))
            {
                NVIC_SystemReset();
            }
            else
            {
                /*设置ID*/
                if ((Uart1.RxBuf[0] == 'A') && (Uart1.RxBuf[1] == 'T') && (Uart1.RxBuf[2] == '+') && (Uart1.RxBuf[3] == 'I') && (Uart1.RxBuf[4] == 'D'))
                {
                    memcpy(g_Equipment_number, &Uart1.RxBuf[5], 16);
                    g_Equipment_number[16] = '\0';
                    // printf("set id\r\n");
                   // FlashWrite_mac_id();
                    NVIC_SystemReset();
                }
                else
                {
                    if ((Uart1.RxBuf[0] == 'A') && (Uart1.RxBuf[1] == 'T') && (Uart1.RxBuf[2] == '+') && (Uart1.RxBuf[3] == 'M') && (Uart1.RxBuf[4] == 'A') && (Uart1.RxBuf[5] == 'C'))
                    {
                       // Ascii2Hex(EtnernetPara.MACAddr, &Uart1.RxBuf[6], 12);
                        // printf("Mac_addr==0x%02x.0x%02x.0x%02x.0x%02x.0x%02x.0x%02x\r\n",EtnernetPara.MACAddr[0],EtnernetPara.MACAddr[1],EtnernetPara.MACAddr[2],EtnernetPara.MACAddr[3],EtnernetPara.MACAddr[4],EtnernetPara.MACAddr[5]);
                        // printf("set mac\r\n");
                      //  FlashWrite_mac_id();
                        NVIC_SystemReset();
                    }
                    else
                    {
                        char *p = (char *)malloc(300 * sizeof(char));
                        if (p != NULL)
                        {
                            sprintf(p, "modbus_status/%s",
                                    g_Equipment_number);
                        }
                        mqtt_publish(&static_client, p, Uart1.RxBuf, Uart1.RxLen, 1, 0, mqtt_pub_request_cb, NULL);
                        Uart1ClearRxBuf();
                        free(p);
                    }
                }
            }
        }
        Uart1ClearRxBuf();
    }
}
