#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main(void)
{
 int sockfd;
 struct  sockaddr_in server_addr;
 char send_buf[]="hello tcp server\n";
 char recv_buf[128]={0};
 int ret;
 
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

ret=connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
if(ret<0)
{
 perror("connect failed");
 close(sockfd);
 return 1;
}
printf("connect server success");

ret=send(sockfd,send_buf,strlen(send_buf),0);
if(ret<0)
{
 perror("send failed");
 close(sockfd);
 return 1;
}
printf("send:%s\n",send_buf);

ret=recv(sockfd,recv_buf,sizeof(recv_buf)-1,0);
if(ret>0)
{
 printf("recv:%s\n",recv_buf);
}
else if(ret==0)
{
 printf("server closed connection\n");
}
else
{
 perror("recv failed");
}
close(sockfd);
return 0;
}

