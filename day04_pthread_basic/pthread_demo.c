#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func(void *arg)
{
 int i=0;
 
 for(i=0;i<5;i++)
 {
  printf("child thread running:%d\n",i);
  sleep(1);
 }
 return NULL;
}
int main(void)
{
 pthread_t tid;
 
 printf("main thread start\n");

 pthread_create(&tid,NULL,thread_func,NULL);
 
 for(int i=0;i<5;i++)
 {
  printf("main thread running:%d\n",i);
  sleep(1);
 }
 pthread_join(tid,NULL);
 
 printf("main thread end\n");

 return 0;
}

