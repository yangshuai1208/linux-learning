#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define SERIAL_DEV "/dev/ttyUSB0"

int serial_init(int fd)
{
  struct termios options;
  
  if(tcgetattr(fd,&options)!=0)
  {
   perror("tcgetattr failed");
   return -1;
  }
  cfsetispeed(&options,B115200);
  cfsetospeed(&options,B115200);
  
  options.c_cflag|=CLOCAL;
  options.c_cflag|=CREAD;
  
  options.c_cflag&=~PARENB;
  options.c_cflag&=~CSTOPB;
  options.c_cflag&=~CSIZE;
  options.c_cflag|=CS8;
  
  options.c_lflag&=~(ICANON|ECHO|ECHOE|ISIG);
  options.c_oflag&=~OPOST;
  
  options.c_cc[VMIN]=0;
  options.c_cc[VTIME]=10;
  
  if(tcsetattr(fd,TCSANOW,&options)!=0)
  {
    perror("tcsetattr failed");
    return -1;
  }
  return 0;
 }
 
 int main(void)
{
  int fd;
  int ret;
  char recv_buf[128];
  
  fd=open(SERIAL_DEV,O_RDWR|O_NOCTTY);
  if(fd<0)
  {
   perror("open serial failed");
   return 1;
  }
  printf("open serial success:%s\n",SERIAL_DEV);
  
  ret=serial_init(fd);
  if(ret!=0)
  {
    close(fd);
    return 1;
  }
  printf("serial init success,waiting stm32 data...\n");
  
  while(1)
  {
    memset(recv_buf,0,sizeof(recv_buf));
    ret=read(fd,recv_buf,sizeof(recv_buf)-1);
    if(ret>0)
    {
      printf("recv from stm32:%s",recv_buf);
    }
    else if(ret==0)
    {
     continue;
    }
    else
    {
      perror("read failed");
      break;
     }
   }
   close(fd);
   return 0;
}
    
  
