/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-5-10      ShiHao       first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "paho_mqtt.h"
#include <wlan_mgnt.h>
#include "aht10.h"
#include "mqtt.h"

#define DBG_TAG "mqtt"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/**
 * MQTT URI farmat:
 * domain mode
 * tcp://broker.emqx.io:1883
 *
 * ipv4 mode
 * tcp://192.168.10.1:1883
 * ssl://192.168.10.1:1884
 *
 * ipv6 mode
 * tcp://[fe80::20c:29ff:fe9a:a07e]:1883
 * ssl://[fe80::20c:29ff:fe9a:a07e]:1884
 */
#define MQTT_URI "tcp://broker.emqx.io:1883"
#define MQTT_SUBTOPIC "/CookGPT/sub/"
#define MQTT_PUBTOPIC "/CookGPT/pub/"

/* define MQTT client context */
static MQTTClient client;

char sup_pub_topic[48] = {0};
char pub_topic[48] = {0};

/* Thread */
#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       1024
#define THREAD_TIMESLICE        5
static rt_thread_t tid1 = RT_NULL;
extern rt_sem_t dynamic_sem;         // 信号量: 收到mqtt信息
/* MQTT */
char messagedate[10];

/* LED */
#define PIN_LED_B              GET_PIN(F, 11)      // PF11 :  LED_B        --> LED
#define PIN_LED_R              GET_PIN(F, 12)      // PF12 :  LED_R        --> LED

void led_thread_entry(void *parameter)
{
    // 设置引脚模式为输出
    rt_pin_mode(PIN_LED_R, PIN_MODE_OUTPUT);

    while (1)
    {
        // 等待信号量
        if (rt_sem_take(dynamic_sem, RT_WAITING_FOREVER) == RT_EOK)
        {
            // 判断消息内容并执行相应操作
            if (strncmp(messagedate, "led_on", strlen("led_on")) == 0)
            {
                rt_pin_write(PIN_LED_R, PIN_LOW);
                rt_kprintf("Led on\r\n");
            }
            else if (strncmp(messagedate, "led_off", strlen("led_off")) == 0)
            {
                rt_pin_write(PIN_LED_R, PIN_HIGH);
                rt_kprintf("Led off\r\n");
            }
        }
    }
}

int mqtt_init(void)
{
    mq_start();

    tid1 = rt_thread_create("mqtt_led",
                            led_thread_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY + 1, THREAD_TIMESLICE);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
}
INIT_APP_EXPORT(mqtt_init);

static void mqtt_sub_callback(MQTTClient *c, MessageData *msg_data)
{
    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
    LOG_D("Topic: %.*s receive a message: %.*s",
          msg_data->topicName->lenstring.len,
          msg_data->topicName->lenstring.data,
          msg_data->message->payloadlen,
          (char *)msg_data->message->payload);

    memcpy(messagedate, msg_data->message->payload, msg_data->message->payloadlen);
    rt_sem_release(dynamic_sem);
    return;
}

static void mqtt_sub_default_callback(MQTTClient *c, MessageData *msg_data)
{
    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
    LOG_D("mqtt sub default callback: %.*s %.*s",
          msg_data->topicName->lenstring.len,
          msg_data->topicName->lenstring.data,
          msg_data->message->payloadlen,
          (char *)msg_data->message->payload);
    return;
}

static void mqtt_connect_callback(MQTTClient *c)
{

    LOG_I("Start to connect mqtt server");
}

static void mqtt_online_callback(MQTTClient *c)
{
    LOG_D("Connect mqtt server success");
    LOG_D("Publish message: Hello,RT-Thread! to topic: %s", pub_topic);
    mq_publish("Hello,RT-Thread!");
}

static void mqtt_offline_callback(MQTTClient *c)
{
    LOG_I("Disconnect from mqtt server");
}

/* 创建与配置 mqtt 客户端 */
void mq_start(void)
{
    /* 初始 condata 参数 */
    MQTTPacket_connectData condata = MQTTPacket_connectData_initializer;
    static char cid[20] = {0};

    static int is_started = 0;
    if (is_started)
    {
        return;
    }
    /* 配置 MQTT 文本参数 */
    {
        client.isconnected = 0;
        client.uri = MQTT_URI;

        /* 生成随机客户端 ID */
        rt_snprintf(sup_pub_topic, sizeof(sup_pub_topic), "%s", MQTT_SUBTOPIC);
        rt_snprintf(pub_topic, sizeof(pub_topic), "%s", MQTT_PUBTOPIC);
        /* 配置连接参数 */
        memcpy(&client.condata, &condata, sizeof(condata));
        client.condata.clientID.cstring = cid;
        client.condata.keepAliveInterval = 60;
        client.condata.cleansession = 1;
        client.condata.username.cstring = "";
        client.condata.password.cstring = "";

        /* 配置 mqtt 参数 */
        client.condata.willFlag = 0;
        client.condata.will.qos = 1;
        client.condata.will.retained = 0;
        client.condata.will.topicName.cstring = sup_pub_topic;

        client.buf_size = client.readbuf_size = 1024;
        client.buf = malloc(client.buf_size);
        client.readbuf = malloc(client.readbuf_size);
        if (!(client.buf && client.readbuf))
        {
            LOG_E("no memory for MQTT client buffer!");
            goto _exit;
        }

        /* 设置事件回调 */
        client.connect_callback = mqtt_connect_callback;
        client.online_callback = mqtt_online_callback;
        client.offline_callback = mqtt_offline_callback;
        /* 设置要订阅的 topic 和 topic 对应的回调函数 */
        client.messageHandlers[0].topicFilter = sup_pub_topic;
        client.messageHandlers[0].callback = mqtt_sub_callback;
        client.messageHandlers[0].qos = QOS1;

        /* 设置默认订阅回调函数 */
        client.defaultMessageHandler = mqtt_sub_default_callback;
    }

    /* 启动 MQTT 客户端 */
    LOG_D("Start mqtt client and subscribe topic:%s", sup_pub_topic);
    paho_mqtt_start(&client);
    is_started = 1;

_exit:
    return;
}

/* MQTT 消息发布函数 */
void mq_publish(const char *send_str)
{
    MQTTMessage message;
    const char *msg_str = send_str;
    const char *topic = pub_topic;
    message.qos = QOS1;
    message.retained = 0;
    message.payload = (void *)msg_str;
    message.payloadlen = strlen(message.payload);

    MQTTPublish(&client, topic, &message);

    return;
}


static void msh_mq_publish(int argc, char *argv[])
{
    char send_buff[100] = {'\0'};
    for(int i=1;i<argc;i++)
    {
        if(i > 1)
        {
            strcat(send_buff," ");
        }
        strcat(send_buff,argv[i]);
    }
    mq_publish(send_buff);  //发布消息给主题"test"
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(msh_mq_publish, publish messege by msh);
