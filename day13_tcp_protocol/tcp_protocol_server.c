#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define SERVER_PORT  8888
#define BACKLOG       5

void handle_command(char *cmd,char *reply,int reply_size)
{
 if(strncmp(cmd,"ping",4)==0)
 {
   snprintf(reply,reply_size,"pong\n");
 }
 else if(strncmp(cmd,"get",3)==0)
 {
   snprintf(reply,reply_size,"temp=25,humi=60,device_id=stm32_01\n");
 }
 else if(strncmp(cmd,"help",4)==0)
 {
  snprintf(reply,reply_size,"commands:ping,get,help\n");
 }
 else 
 {
   snprintf(reply,reply_size,"unknown command\n");
 }
}
void *client_thread(void *arg)
 {
  int connfd=*(int *)arg;
  free(arg);
  
  int ret;
  char recv_buf[128]={0};
  char send_buf[128]={0};
  
  printf("client thread start,connfd=%d\n",connfd);
  
 while(1)
 {
  memset(recv_buf,0,sizeof(recv_buf));
  memset(send_buf,0,sizeof(send_buf));
  
  ret=recv(connfd,recv_buf,sizeof(recv_buf)-1,0);
  if(ret>0)
  {
    printf("recv from connfd %d:%s",connfd,recv_buf);
    handle_command(recv_buf,send_buf,sizeof(send_buf));
    send(connfd,send_buf,strlen(send_buf),0);    
}
  else if(ret==0)
  {
   printf("client disconnected,connfd=%d\n",connfd);
   break;
  }
  else
  {
   perror("recv failed");
   break;
  }
 }
  close(connfd);
  return NULL;
}
int main(void)
 {
  int listenfd;
  int connfd;
  int ret;
  int opt;
  pthread_t tid;
  
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  socklen_t client_len;
  
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
    printf("tcp protocol server listening on port %d...\n",SERVER_PORT);
    
   while(1)
   {
     client_len=sizeof(client_addr);
     connfd=accept(listenfd,(struct sockaddr*)&client_addr,&client_len);
     if(connfd<0)
     {
      perror("accept failed");
      continue;
     }
     printf("new client connected,connfd=%d\n",connfd);
     
     int *pconnfd=malloc(sizeof(int));
     if(pconnfd==NULL)
     {
      perror("malloc failed");
      close(connfd);
      continue;
     }
     *pconnfd=connfd;
     
     ret=pthread_create(&tid,NULL,client_thread,pconnfd);
     if(ret!=0)
     {
       perror("pthread_create failed");
       close(connfd);
       free(pconnfd);
       continue;
     }
      pthread_detach(tid);
   }
   close(listenfd);
   return 0;
}

