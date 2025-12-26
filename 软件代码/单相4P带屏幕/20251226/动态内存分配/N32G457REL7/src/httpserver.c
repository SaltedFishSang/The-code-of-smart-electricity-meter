/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file httpserver.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */

#include "lwip/netif.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/tcp.h"
#include "log.h"

#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG LWIP_DBG_OFF
#endif

/*Function declaration*/
void http_server_init(void);
err_t http_accept(void *arg, struct tcp_pcb *pcb, err_t err);
err_t http_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);

/*Define and Initialize the HTML header and page*/
static const char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
static const char http_index_html[] =
    "<html><head><title>Congrats!</title></head><body><h1>Welcome to our lwIP HTTP server!</h1><p>This is a small test "
    "page, served by httpserver-netconn.<test>From nation N32G457QEL_EVB v1.1!</body></html>";

struct TCP_STATE_STR{
    u8_t state;
    struct tcp_pcb *pcb;
    /* pbuf (chain) to recycle */
    struct pbuf *p;
};

/**
 * @brief tcp_recv callback functions:Data has been received on this pcb.
 */
err_t http_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    err_t ret_err;
    struct TCP_STATE_STR *ts;
    char *pData = NULL;
    if ((arg == NULL) || (pcb == NULL))
    {
        return ERR_VAL;
    }
    ts = (struct TCP_STATE_STR *)arg;
    
    if(p != NULL)
    {
        /*Update the receive structure*/
        tcp_recved(pcb, p->tot_len);
        pData = p->payload;
        
        /* Is this an HTTP GET command? (only check the first 5 chars, since
        there are other formats for GET, and we're keeping it very simple )*/
        if(p->len >= 5 && pData[0] == 'G' && pData[1] == 'E' && pData[2] == 'T' && pData[3] == ' ' && pData[4] == '/')
        {
            /* Send the HTML header
             * subtract 1 from the size, since we dont send the \0 in the string
             * NETCONN_NOCOPY: our data is const static, so no need to copy it
             */
            tcp_write(pcb,http_html_hdr,sizeof(http_html_hdr),1);
            /* Send our HTML page */
            tcp_write(pcb,http_index_html, sizeof(http_index_html),1);
        }
        pbuf_free(p);
        
        mem_free(ts);
        tcp_close(pcb);
        
        ret_err = ERR_OK;
    }
//    else
//    {
//        /*The client is disconnected*/
//        mem_free(ts);
//        tcp_close(pcb);
//        ret_err = ERR_CLSD;
//    }
    return ret_err;
}

/**
 * @brief tcp_accept callback functions:A new incoming connection has been accepted.
 */
err_t http_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
    err_t ret_err;
    struct TCP_STATE_STR *ts;
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);
    if ((err != ERR_OK) || (pcb == NULL))
    {
        return ERR_VAL;
    }
    
    /* Set priority */
    tcp_setprio(pcb, TCP_PRIO_MIN);
    
    ts = (struct TCP_STATE_STR *)mem_malloc(sizeof(struct TCP_STATE_STR));
    if(ts != NULL)
    {
        ts->pcb = pcb;
        ts->p = NULL;
        tcp_arg(pcb, ts);
        /* Set up the various callback functions */
        tcp_recv(pcb, http_recv);
        //tcp_err(pcb, http_err);
        //tcp_poll(pcb, http_poll, 4);
        //tcp_sent(pcb, http_sent);
        ret_err = ERR_OK;
    }
    else
    {
        ret_err = ERR_MEM;
    }
    return ret_err;
}

/**
 * @brief Initialize the http server(NO SYS).
 */
void http_server_init(void)
{
    struct tcp_pcb *pcb;
    err_t err;
    
    /*Creates a new TCP protocol control block.*/
    pcb = tcp_new();
    LWIP_ASSERT("http_server_init: tcp_new failed", pcb != NULL);
    if(pcb)
    {
        /*Sets the priority of a connection.*/
        tcp_setprio(pcb, TCP_PRIO_MIN);
        /*Binds the connection to a local port number and IP address.*/
        err = tcp_bind(pcb, IP_ANY_TYPE, 80);
        LWIP_ASSERT("http_server_init: tcp_bind failed", err == ERR_OK);
        /*Set the state of the connection to be LISTEN*/
        pcb = tcp_listen(pcb);
        LWIP_ASSERT("http_server_init: tcp_listen failed", pcb != NULL);
        /*Tell TCP that this is the structure we wish to be passed for our callbacks.*/
        tcp_arg(pcb, pcb);
        /*Used for specifying the function that should be called 
          when a LISTENing connection has been connected to another host.*/
        tcp_accept(pcb, http_accept);
    }
}

