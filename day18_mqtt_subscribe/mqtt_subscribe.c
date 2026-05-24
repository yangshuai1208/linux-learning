#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>

#define MQTT_HOST "localhost"
#define MQTT_PORT 1883
#define MQTT_KEEPALIVE 60

#define MQTT_TOPIC "gateway/stm32_01/sensor"

void on_connect(struct mosquitto*mosq,void *userdata,int rc)
{
  int ret;
  
  if(rc==0)
  {
    printf("connect broker success\n");
     
    ret=mosquitto_subscribe(mosq,NULL,MQTT_TOPIC,0);
    if(ret!=MOSQ_ERR_SUCCESS)
    {
      printf("subscribe failed:%s\n",mosquitto_strerror(ret));
    }
    else
    {
      printf("subscribe topic:%s\n",MQTT_TOPIC);
    }
  }
   else
    {
      printf("connect broker failed,rc=%d\n",rc);
    }
}
void on_message(struct mosquitto*mosq,void *userdata,const struct mosquitto_message *msg)
{
  printf("recv mqtt message\n");
  printf("topic:%s\n",msg->topic);
  printf("payload:%.*s\n",msg->payloadlen,(char*)msg->payload);
}
int main(void)
{
  struct mosquitto*mosq=NULL;
  int ret;
  
  mosquitto_lib_init();
  
  mosq=mosquitto_new("gateway_sub_client",true,NULL);
  if(mosq==NULL)
  {
    printf("mosquitto_new failed\n");
    mosquitto_lib_cleanup;
    return 1;
  }
  mosquitto_connect_callback_set(mosq,on_connect);
  mosquitto_message_callback_set(mosq,on_message);
  
  ret=mosquitto_connect(mosq,MQTT_HOST,MQTT_PORT,MQTT_KEEPALIVE);
  if(ret!=MOSQ_ERR_SUCCESS)
  {
   printf("connect failed:%s\n",mosquitto_strerror(ret));
   mosquitto_destroy(mosq);
   mosquitto_lib_cleanup();
   return 1;
  }

  printf("mqtt subscribe client started\n");
  
  ret=mosquitto_loop_forever(mosq,-1,1);
  if(ret!=MOSQ_ERR_SUCCESS)
  {
   printf("loop failed:%s\n",mosquitto_strerror(ret));
  }
  
  mosquitto_destroy(mosq);
  mosquitto_lib_cleanup();
  
  return 0;
}
