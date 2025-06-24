#ifndef _MQTT_H_
#define _MQTT_H_


void mq_start(void);

/* MQTT 消息发布函数 */
void mq_temp_publish(const char *send_str);
void mq_wet_publish(const char *send_str);
void mq_fire_publish(const char *send_str);
void mq_smoke_publish(const char *send_str);


#endif
