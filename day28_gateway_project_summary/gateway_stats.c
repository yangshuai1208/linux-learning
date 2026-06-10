#include <stdio.h>
#include <time.h>

#include "gateway_stats.h"

void gateway_stats_init(GatewayStats*stats)
{
  if(stats==NULL)
  {
    return;
  }
  stats->total_count=0;
  stats->parse_success_count=0;
  stats->parse_fail_count=0;
  stats->mqtt_success_count=0;
  stats->mqtt_fail_count=0;
}

void gateway_stats_print(const GatewayStats*stats)
{
 if(stats==NULL)
 {
  return;
 }

 printf("==============gateway stats===================\n");
 printf("total count     :%d\n",stats->total_count);
 printf("parse success count :%d\n",stats->parse_success_count);
 printf("parse fail count:%d\n",stats->parse_fail_count);
 printf("mqtt success count:%d\n",stats->mqtt_success_count);
 printf("mqtt fail count:%d\n",stats->mqtt_fail_count);
 printf("==============================================\n");
}

int gateway_stats_save_report(const GatewayStats*stats)
{
 FILE*fp=NULL;
 time_t now;
 struct tm*time_info;
 char time_buf[64];
 
 if(stats==NULL)
 {
   return -1;
 }

fp=fopen("gateway_report.txt","w");
if(fp==NULL)
{
 perror("fopen gateway_report.txt failed");
 return -1;
}
now=time(NULL);
time_info=localtime(&now);

strftime(time_buf,sizeof(time_buf),"%Y-%m-%d %H:%M:%S",time_info);

fprintf(fp,"Gateway Run Report\n");
fprintf(fp,"Time:%s\n",time_buf);
fprintf(fp,"--------------------------\n");
fprintf(fp,"total count     :%d\n",stats->total_count);
fprintf(fp,"parse success count :%d\n",stats->parse_success_count);
fprintf(fp,"parse fail count:%d\n",stats->parse_fail_count);
fprintf(fp,"mqtt success count:%d\n",stats->mqtt_success_count);
fprintf(fp,"mqtt fail count:%d\n",stats->mqtt_fail_count);
fprintf(fp,"-------------------------\n");

fclose(fp);
return 0;
}
