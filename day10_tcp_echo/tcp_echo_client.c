#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT  8888

int main(void)
{
 int sockfd;
 int ret;
  
 struct sockaddr_in server_addr;
 
 char send_buf[128]={0};
 char recv_buf[128]={0};
 
 sockfd=socket(AF_INET,SOCK_STREAM,0);
 if(sockfd<0)
 {
  perror("socket failed");
  return 1;
 }
 memset(&server_addr,0,sizeof(server_addr));
 
 server_addr.sin_family=AF_INET;
 server_addr.sin_port=htons(SERVER_PORT);
 
 ret=inet_pton(AF_INET,SERVER_IP,&server_addr.sin_addr);
 if(ret<=0)
 {
  perror("inet_pton failed");
  close(sockfd);
  return 1;
  }
 
 ret=connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
 if(ret<0)
 {
  perror("connect failed");
  close(sockfd);
  return 1;
 }
 printf("connect server success\n");
 while(1)
 {
  memset(send_buf,0,sizeof(send_buf));
  memset(recv_buf,0,sizeof(recv_buf));

  printf("input message:");
  if(fgets(send_buf,sizeof(send_buf),stdin)==NULL)
   {
     break;
   }
    if(strncmp(send_buf,"quit",4)==0)
    {
       break;
    }
    ret=send(sockfd,send_buf,strlen(send_buf),0);
    if(ret<0)
     {
       perror("send failed");
       break;
     }
     ret=recv(sockfd,recv_buf,sizeof(recv_buf)-1,0);
     if(ret>0)
     {
      printf("echo from server:%s",recv_buf);
     }
     else if(ret==0)
     {
       printf("server closed\n");
       break;
     }
     else
     {
      perror("recv failed");
      break;
   }
  }
  close(sockfd);
  return 0;
}

