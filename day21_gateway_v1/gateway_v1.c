#include <stdio.h>
#include <string.h>
#include <time.h>

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
int build_json(const SensorData*data,char *json_buf,int buf_size)
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
 
 strftime(time_buf,sizeof(time_buf),"%Y-%m_%d %H:%M:%S",time_info);
 fprintf(fp,"[%s]%s\n",time_buf,json_data);
 
 fclose(fp);
 return 0;
}
int main(void)
{
  const char*raw_data="temp=25,humi=60,device_id=stm32_01";
  SensorData sensor;
  char json_buf[128];
  int ret;
  
  printf("gateway v1 start\n");
 
 printf("raw data:%s\n",raw_data);
 
 ret=parse_sensor_data(raw_data,&sensor);
 if(ret!=0)
 {
  printf("parse sensor data failed\n");
  return 1;
 }
 printf("parse success");
 
 ret=build_json(&sensor,json_buf,sizeof(json_buf));
 if(ret!=0)
 {
  printf("build json failed\n");
  return 1;
}
 printf("json:%s\n",json_buf);
 
 ret=write_log(json_buf);
 if(ret!=0)
 {
  printf("write log failed");
  return 1;
 }
 printf("write log success\n");
 
 return 0;
}

 
