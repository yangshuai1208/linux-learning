#include <stdio.h>
#include <string.h>

int main(void)
{
 FILE*fp=NULL;
 char buffer[128]={0};
 
 fp=fopen("sensor_log.txt","w");
 if(fp==NULL)
 {
  printf("open file for write failed\n");
  return 1;
 }
 fprintf(fp,"device_id=stm32_01,temp=25,humi=60\n");
 fclose(fp);
 
 fp=fopen("sensor_log.txt","r");
 if(fp==NULL)
 {
  printf("open file for read failed\n");
  return 1;
 }
 fgets(buffer,sizeof(buffer),fp);
 fclose(fp);

 printf("read from file:%s",buffer);

 return 0;
}
