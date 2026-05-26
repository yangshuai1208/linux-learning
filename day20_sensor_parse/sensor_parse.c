#include <stdio.h>
#include <string.h>

typedef struct
{
 int temp;
 int humi;
 char device_id[32];
}SensorData;

int parse_sensor_data(const char*raw,SensorData*data)
{
 int ret;
 
 if(raw==NULL||data==NULL)
 {
   return -1;
 }
 memset(data,0,sizeof(SensorData));
 ret=sscanf(raw,
            "temp=%d,humi=%d,device_id=%31s", 
             &data->temp,
             &data->humi,
             data->device_id);
  if(ret!=3)
  {
    return -1;
  }
  return 0;
 }
int build_json(const SensorData*data,char*json_buf,int buf_size)
{
  if(data==NULL||json_buf==NULL||buf_size<=0)
   {
     return -1;
   }
    snprintf(json_buf,
             buf_size,
             "{\"temp\":%d,\"humi\":%d,\"device_id\":\"%s\"}",
              data->temp,
              data->humi,
              data->device_id);
        return 0;
 }
 int main(void)
 {
   const char*raw_data="temp=25,humi=60,device_id=stm32_01";
   SensorData sensor;
   char json_buf[128];
   int ret;
   
   printf("raw data:%s\n",raw_data);
   
   ret=parse_sensor_data(raw_data,&sensor);
   if(ret!=0)
   {
    printf("parse sensor data failed\n");
    return 1;
   }
   
   printf("parse success\n");
   printf("temp:%d\n",sensor.temp);
   printf("humi:%d\n",sensor.humi);
   printf("device_id:%s\n",sensor.device_id); 

   ret=build_json(&sensor,json_buf,sizeof(json_buf));
   if(ret!=0)
   {
    printf("build json failed\n");
    return 1;
   }
   printf("json:%s\n",json_buf);
  
   return 0;
}
   
 
