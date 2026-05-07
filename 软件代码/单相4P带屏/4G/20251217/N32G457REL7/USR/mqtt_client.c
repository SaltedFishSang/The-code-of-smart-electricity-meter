#include "BSP.h"
ip_addr_t domain_ipaddr;
mqtt_client_t static_client;
mqtt_client_t *client11;
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags);
void mqtt_pub_request_cb(void *arg, err_t result);
static void mqtt_sub_request_cb(void *arg, err_t result);
struct mqtt_connect_client_info_t ci;
void example_do_connect(mqtt_client_t *client)
{
    err_t err;
    memset(&ci, 0, sizeof(ci));
    ci.client_id = g_Equipment_number;
    ci.client_user = "user";
    ci.client_pass = "0.123456";
    ci.keep_alive = 60;
    err = mqtt_client_connect(client, &domain_ipaddr, EtnernetPara.DESPORT, mqtt_connection_cb, 0, &ci);
    if (err != ERR_OK)
    {
#if _Printf_DBG
        printf("mqtt_connect return %d\n", err);
#endif
    }
}

/*!
    \brief      callback for connection status, called when client has connected to the server
                after initiating a mqtt connection attempt by calling mqtt_connect() or when
                connection is closed by server or an error
    \param[in]  client: MQTT client itself
    \param[in]  arg: additional argument to pass to the callback function
    \param[in]  status: connect result code or disconnection notification
    \param[out] none
    \retval     none
*/
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{

    err_t err;
    u8 table[7];
    char p[50], p1[50];
    sprintf(p, "modbus_control/%s", g_Equipment_number);
    table[0] = g_Equipment_number[0];
    table[1] = g_Equipment_number[2];
    table[2] = '\0';
    sprintf(p1, "modbus_control/%s", table);
    if (status == MQTT_CONNECT_ACCEPTED)
    {
        g_mqtt_errnum = 0;
        NETLED_ON;
        HeartLimit();
#if _Printf_DBG
        printf("mqtt_connection_cb: Successfully connected\r\n");
#endif
        mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);

			
			
			
/*************************¶©ÔÄmodbus_control/<id>***************************/
			  err = mqtt_subscribe(client, p, 1, mqtt_sub_request_cb, arg);
        if (err != ERR_OK)
        {
#if _Printf_DBG
            printf("mqtt_subscribe electric/control/ return: %d\n", err);
#endif
        }
        err = mqtt_subscribe(client, p1, 1, mqtt_sub_request_cb, arg);
        if (err != ERR_OK)
        {
#if _Printf_DBG
            printf("mqtt_subscribe electric/control1/ return: %d\n", err);
#endif
        }
				
			
/*************************¶©ÔÄmodbus_time/<id>***************************/				
				char sub_time[50];
				sprintf(sub_time, "modbus_time/%s", g_Equipment_number);
        err = mqtt_subscribe(client, sub_time, 1, mqtt_sub_request_cb, arg);
        if (err != ERR_OK)
        {
#if _Printf_DBG
            printf("mqtt_subscribe electric/control1/ return: %d\n", err);
#endif
        }	
    }
    else
    {
#if _Printf_DBG
        printf("mqtt_connection_cb: Disconnected, reason: %d     \n", status);
#endif
        MQTT_Client();
    }
}
u32 m_mqttreclen = 0;

/*!
    \brief      callback for incoming publish, called when an incoming publish arrives to a subscribed topic
    \param[in]  arg: additional argument to pass to the callback functionn
    \param[in]  topic: '}' terminated Topic text string, topic may not be referenced after callback return
    \param[in]  tot_len: total length of publish data, if set to 0 (no publish payload) data callback will not be invoked
    \param[out] none
    \retval     none
*/
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
#if _Printf_DBG
    printf("topic is :%s with total length %u\r\n", topic, (unsigned int)tot_len);
    printf("\r\n");
#endif
    m_mqttreclen = tot_len;
    /* decode topic string into a user defined reference */
    if (strstr(topic, "control") != NULL)
    {
    }
}

/*!
    \brief      callback for incoming publish data, called when data arrives to a subscribed topic
    \param[in]  arg: additional argument to pass to the callback function
    \param[in]  data: user data, pointed object, data may not be referenced after callback return,
                NULL is passed when all publish data are delivered
    \param[in]  len: length of publish data fragment
    \param[in]  flags: MQTT_DATA_FLAG_LAST set when this call contains the last part of data from publish message
    \param[out] none
    \retval     none
*/
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    u8 sendtable[100];
#if _Printf_DBG
    printf("Incoming publish payload with length %d, flags %u\n", len, (unsigned int)flags);
    printf("%s\r\n", data);
#endif
    if (flags & MQTT_DATA_FLAG_LAST)
    {
        if (m_mqttreclen > 0)
        {
            g_mqtt_errnum = 0;
            memcpy(sendtable, data, m_mqttreclen);
            Uart1Send(sendtable, m_mqttreclen);
            m_mqttreclen = 0;
        }
    }
    else
    {
        /* Handle fragmented payload, store in buffer, write to file or whatever */
    }
}

/*!
    \brief      mqtt publish function
    \param[in]  client: MQTT client
    \param[in]  arg: user supplied argument to publish callback
    \param[in]  topic: publish topic string
    \param[in]  message: data to publish (NULL is allowed)
    \param[out] none
    \retval     none
*/
void new_meg_publish(mqtt_client_t *client, void *arg, char *topic, char *message, char retain)
{
    err_t err;
    u8_t qos = 1; /* 0 1 or 2, see MQTT specification */
    // u8_t retain = 1; /* no don't retain such crappy payload... */
    err = mqtt_publish(client, topic, message, strlen(message), qos, retain, mqtt_pub_request_cb, arg);
    if (err == ERR_OK)
    {
#if _Printf_DBG
        printf("publish ok \r\n");
#endif
    }
}

/*!
    \brief      callback for mqtt publish, when publish is complete either with sucess or failure
    \param[in]  arg: pointer to user data supplied when invoking request
    \param[in]  result
    \arg        ERR_OK on success
    \arg        ERR_TIMEOUT if no response was received within timeout,
    \arg        ERR_ABRT if (un)subscribe was denied
    \param[out] none
    \retval     none
*/
void mqtt_pub_request_cb(void *arg, err_t result)
{
    if (result != ERR_OK)
    {
#if _Printf_DBG
        printf("Publish result: %d\n", result);
#endif
    }
}

/*!
    \brief      callback for mqtt request
    \param[in]  arg: pointer to user data supplied when invoking request
    \param[in]  result
    \arg        ERR_OK on success
    \arg        ERR_TIMEOUT if no response was received within timeout,
    \arg        ERR_ABRT if (un)subscribe was denied
    \param[out] none
    \retval     none
*/
static void mqtt_sub_request_cb(void *arg, err_t result)
{
    /* Just print the result code here for simplicity,
       normal behaviour would be to take some action if subscribe fails like
       notifying user, retry subscribe or disconnect from server */
#if _Printf_DBG
    printf("Subscribe result: %d\n", result);
#endif
}
