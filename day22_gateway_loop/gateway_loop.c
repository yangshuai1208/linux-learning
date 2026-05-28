#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
 int temp;
 int humi;
 char device_id[32];
}SensorData;
int parse_sensor_data(const char*raw,SensorData *data)
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
int build_json(const SensorData *data,char *json_buf,int buf_size)
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
int write_log(const char*json_data)
{
 FILE*fp=NULL;
 time_t now;
 struct tm*time_info;
 char time_buf[64];
 
 if(json_data==NULL)
 {
  return -1;
 }
 fp=fopen("sensor_log.txt","a");
 if(fp==NULL)
{
 perror("fopen sensor_log.txt failed");
 return -1;
}
 now=time(NULL);
 time_info=localtime(&now);
 
 strftime(time_buf,sizeof(time_buf),"%Y-%m-%d %H:%M:%S",time_info);
 fprintf(fp,"[%s]%s\n",time_buf,json_data);
 fclose(fp);
 return 0;
}
int main(void)
{
 const char*raw_data_list[]=
 {
  "temp=25,humi=60,device_id=stm32_01",
  "temp=25,humi=60,device_id=stm32_01",
  "temp=25,humi=60,device_id=stm32_01",
  "temp=25,humi=60,device_id=stm32_01",
  "temp=25,humi=60,device_id=stm32_01"
};
int data_count=sizeof(raw_data_list)/sizeof(raw_data_list[0]);
int i;
int ret;
SensorData sensor;
char json_buf[128];

printf("gateway loop start\n");

for(i=0;i<data_count;i++)
{
 printf("-------------------\n");
 printf("raw data:%s\n",raw_data_list[i]);

 ret=parse_sensor_data(raw_data_list[i],&sensor);
 if(ret!=0)
 {
  printf("parse sensor data failed\n");
  continue;
 }
 printf("parse success\n");
 printf("temp:%d\n",sensor.temp);
 printf("humi:%d\n",sensor.humi);
 printf("device_id:%s\n",sensor.device_id);

 ret=build_json(&sensor,json_buf,sizeof(json_buf));
 if(ret!=0)
 {
  printf("build json failed");
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
 
 sleep(2);
}
 printf("gateway loop end\n");
 
 return 0;
}
