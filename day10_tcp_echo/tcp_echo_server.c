#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_PORT 8888
#define BACKLOG 5

int main(void)
{
 int listenfd;
 int connfd;
 int ret;
 int opt=1;

 struct sockaddr_in server_addr;
 struct sockaddr_in client_addr;
 socklen_t client_len=sizeof(client_addr);
 
 char recv_buf[128]={0};

 listenfd=socket(AF_INET,SOCK_STREAM,0);
 if(listenfd<0)
 {
  perror("socket failed");
  return 1;
 }
 setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
 
 memset(&server_addr,0,sizeof(server_addr));

 server_addr.sin_family=AF_INET;
 server_addr.sin_port=htons(SERVER_PORT);
 server_addr.sin_addr.s_addr=INADDR_ANY;

 ret=bind(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
 if(ret<0)
 {
  perror("bind failed");
  close(listenfd);
  return 1;
 }

 ret=listen(listenfd,BACKLOG);
 if(ret<0)
 {
  perror("listen failed");
  close(listenfd);
  return 1;
 }
 printf("tcp echo server listening on port%d...\n",SERVER_PORT);
 
  connfd=accept(listenfd,(struct sockaddr*)&client_addr,&client_len);
  if(connfd<0)
  {
   perror("accept failed");
   close(listenfd);
   return 1;
  }
  printf("client connected\n");

 while(1)
 {
  memset(recv_buf,0,sizeof(recv_buf));
  ret=recv(connfd,recv_buf,sizeof(recv_buf)-1,0);
  if(ret>0)
 {
  printf("recv:%s",recv_buf);
  send(connfd,recv_buf,strlen(recv_buf),0);
}
 else if(ret==0)
  {
   printf("client disconnected\n");
   break;
 }
  else
 {
  perror("recv failed");
  break;
 }
}
 close(connfd);
 close(listenfd);

 return 0;
}
