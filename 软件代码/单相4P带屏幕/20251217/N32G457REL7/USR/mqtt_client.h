/*!
    \file  mqtt.h
    \brief the header file of mqtt 
*/

/*
    Copyright (C) 2016 GigaDevice

    2016-08-15, V1.0.0, demo for GD32F4xx
*/
#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "lwip/apps/mqtt.h"
extern mqtt_client_t static_client;
 extern mqtt_client_t *client11;
extern ip_addr_t domain_ipaddr;
/* function declarations */
/* sign in a client, and connect the client to mqtt server(blocking) */
void example_do_connect(mqtt_client_t *client);
/* publish a message to mqtt server */
void new_meg_publish(mqtt_client_t *client, void *arg, char *topic, char *message,char retain);
/* the function handler of topic "pyboard_led" */
void remote_led(char* data);
/* the function handler of topic "pyboard_value" */
void remote_value(char* data);
void remote_relay(char* data);
extern void mqtt_pub_request_cb(void *arg, err_t result);
#endif /* MQTT_CLIENT_H */
