//#include "bsp.h"
//char UdpSendBuffer[500]={"Sang jian First Lwip UDP test"};
//struct udp_pcb *UdpPCB;
//ip_addr_t DestIPaddr;
//struct pbuf *table111;

////void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
////{
////	//char *recdata=0;
////	/*increment message count */
//// // message_count++;
//////	printf("收到数据\r\n");
////	if(p !=NULL)
////	{		
////		#if 0
////		recdata=(char *)malloc(p->len*sizeof(char));
////		if(recdata!=NULL)
////		{
////			memcpy(recdata,p->payload,p->len);
////			printf("upd_rec:%s",recdata); 
////		}
////		free(recdata);
////    #endif
////	//  udpsend(p->payload,p->tot_len);
////		
////		sadp_recv(p,addr,port);
////		/* Free receive pbuf */
////		pbuf_free(p);
////		

////	}
////  
//////  /* free the UDP connection, so we can accept new clients */
//////  udp_remove(upcb);   
////}
//void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, ip_addr_t *addr, u16_t port)
//{
//	//char *recdata=0;
//	/*increment message count */
// // message_count++;
//	printf("收到数据\r\n");
//	if(p !=NULL)
//	{		
//		#if 0
//		recdata=(char *)malloc(p->len*sizeof(char));
//		if(recdata!=NULL)
//		{
//			memcpy(recdata,p->payload,p->len);
//			printf("upd_rec:%s",recdata); 
//		}
//		free(recdata);
//    #endif
//	//  udpsend(p->payload,p->tot_len);
//		
//		sadp_recv(p,addr,port);
//		/* Free receive pbuf */
//		pbuf_free(p);
//		

//	}
//  
////  /* free the UDP connection, so we can accept new clients */
////  udp_remove(upcb);   
//}

//void UdpDataSend(unsigned short len)
//{
//	    ip_addr_t destIPaddr;
//	    struct pbuf *ptr=NULL;
//	    ptr = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
//	    memset(ptr->payload, 0, len);
//      memcpy(ptr->payload, UdpSendBuffer, len);
//	  // IP4_ADDR(&destIPaddr, EtnernetPara.DESIP[0], EtnernetPara.DESIP[1], EtnernetPara.DESIP[2], EtnernetPara.DESIP[3] );
//	    IP4_ADDR( &destIPaddr, 192, 168, 1, 221 );
//			udp_sendto(UdpPCB, ptr,&destIPaddr,8080);   
//			pbuf_free(ptr);   
//	    printf("udp  发送数据\r\n");
//}

//void udpsend(u8_t *bySendBuf, u16_t wSendLen)
//{
//	  ip_addr_t destIPaddr;
//		struct pbuf *ptr=NULL;
//		ptr = pbuf_alloc(PBUF_TRANSPORT, wSendLen, PBUF_RAM);
//	  memset(ptr->payload, 0, wSendLen);
//    memcpy(ptr->payload, bySendBuf, wSendLen);
//	  IP4_ADDR( &destIPaddr, 239, 255, 255, 250 );
//	  udp_sendto(UdpPCB, ptr,&destIPaddr,37020);   
//		pbuf_free(ptr);    
//}
//void udp_echoclient_connect(void)
//{
//    struct udp_pcb *udppcb;  
//    udppcb = udp_new();
//	  UdpPCB = udppcb;
//   // IP4_ADDR( &local_addr, 192, 168, 1 ,152 );
//    udp_bind(udppcb, IP_ADDR_ANY, 37020);
//	//	udp_connect(udppcb,&DestIPaddr,37020);
//    udp_recv(udppcb,udp_receive_callback,NULL);
//	  printf("UDP Start\r\n");
//	  UdpDataSend(100);
//}

//void udp_echoclient_disconnect(void)
//{
//	#if 1
//	printf("Disconnect to UDP echo server");
//	#endif
//	 /* free the UDP connection, so we can accept new clients */
//  udp_remove(UdpPCB);
//}

