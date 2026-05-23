#include <stdio.h>
#include <string.h>
#include <mosquitto.h>
#include <stdbool.h>

#define MQTT_HOST "localhost"
#define MQTT_PORT 	1883
#define MQTT_KEEPALIVE   60

#define MQTT_TOPIC "geteway/stm32_01/sensor"

int main(void)
{
  struct mosquitto *mosq=NULL;
  int ret;
  
  const char*payload="{\"temp\":25,\"humi\":60,\"device_id\":\"stm32_01\"}";
  
  mosquitto_lib_init();
  
  mosq=mosquitto_new("gateway_pub_client",true,NULL);
  if(mosq==NULL)
  {
   printf("mosquitto_new failed\n");
   mosquitto_lib_cleanup();
   return 1;
  }
  ret=mosquitto_connect(mosq,MQTT_HOST,MQTT_PORT,MQTT_KEEPALIVE);
  if(ret!=MOSQ_ERR_SUCCESS)
  {
   printf("connect failed:%s\n",mosquitto_strerror(ret));
   mosquitto_destroy(mosq);
   mosquitto_lib_cleanup();
   return 1;
  }
  
  ret=mosquitto_publish(
  mosq,
  NULL,
  MQTT_TOPIC,
  strlen(payload),
  payload,
  0,
  false
);
  if(ret!=MOSQ_ERR_SUCCESS)
  {
    printf("publish failed:%s\n",mosquitto_strerror(ret));
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 1;
   }
  
  mosquitto_loop(mosq,1000,1);
  
  printf("mqtt publish successs\n");
  printf("topic:%s\n",MQTT_TOPIC);
  printf("payload:%s\n",payload);
  
  mosquitto_disconnect(mosq);
  mosquitto_destroy(mosq);
  mosquitto_lib_cleanup();

  return 0;
}

  
