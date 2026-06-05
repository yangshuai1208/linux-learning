#include <stdio.h>
#include <unistd.h>
#include <mosquitto.h>

#include "sensor_parser.h"
#include "log_writer.h"
#include "mqtt_client.h"

int main(void)
{
 const char*raw_data_list[]=
{
 "temp=25,humi=60,device_id=stm32_01",
 "temp=26,humi=61,device_id=stm32_01",
 "temp=27,humi=62,device_id=stm32_01",
 "temp=28,humi=63,device_id=stm32_01",
 "temp=29,humi=64,device_id=stm32_01"
};
int data_count=sizeof(raw_data_list)/sizeof(raw_data_list[0]);
int i;
int ret;

SensorData sensor;
char json_buf[128];

struct mosquitto *mosq=NULL;
printf("gateway module start\n");

ret=mqtt_client_init(&mosq);
if(ret!=0)
{
 printf("mqtt client init failed\n");
 return -1;
}
for(i=0;i<data_count;i++)
{
  printf("-----------------\n");
  printf("raw data:%s\n",raw_data_list[i]);
  
  ret=parse_sensor_data(raw_data_list[i],&sensor);
  if(ret!=0)
  {
   printf("parse sensor data failed\n");
   continue;
  }
 printf("parse success\n");
  
 ret=build_json(&sensor,json_buf,sizeof(json_buf));
 if(ret!=0)
 {
   printf("build json failed\n");
   continue;
 }
 printf("json:%s\n",json_buf);
 
 ret=write_log(json_buf);
 if(ret!=0)
 {
  printf("write log failed\n");
  continue;
 }
 printf("write log success\n");
 
 ret=mqtt_publish_json(mosq,json_buf);
 if(ret!=0)
 {
  printf("mqtt publish json failed\n");
  continue;
 }
 printf("mqtt publish success\n");
 
 sleep(2);
}
mqtt_client_cleanup(mosq);
printf("gateway module end\n");
return 0;
}
  
