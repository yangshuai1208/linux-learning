#include <stdio.h>
#include <unistd.h>
#include <mosquitto.h>

#include "config.h"
#include "data_source.h"
#include "sensor_parser.h"
#include "log_writer.h"
#include "mqtt_client.h"

int main(void)
{

int i;
int ret;


char raw_buf[128];
char json_buf[128];

SensorData sensor;
struct mosquitto *mosq=NULL;

printf("gateway module start\n");

ret=data_source_init();
if(ret!=0)
{
 printf("data source init failed\n");
 return 1;
}


ret=mqtt_client_init(&mosq);
if(ret!=0)
{
 printf("mqtt client  init failed\n");
 data_source_cleanup();
 return 1;
}

for(i=0;i<LOOP_COUNT;i++)
{
  printf("-----------------\n");
  
  
  ret=data_source_read(raw_buf,sizeof(raw_buf));
  if(ret!=0)
  {
   printf("read data source failed\n");
   continue;
  }
 printf("raw data:%s\n",raw_buf);

 ret=parse_sensor_data(raw_buf,&sensor);
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
  
