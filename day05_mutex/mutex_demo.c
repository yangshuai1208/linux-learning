#include <stdio.h>
#include <pthread.h>

#define LOOP_COUNT 100000

int g_counter=0;

pthread_mutex_t g_mutex;

void *thread_func(void *arg)
{
 int i=0;
 for(i=0;i<LOOP_COUNT;i++)
 {
  pthread_mutex_lock(&g_mutex);
 
 g_counter++;
 pthread_mutex_unlock(&g_mutex);
 }
 return NULL;
}
int main(void)
{
 pthread_t tid1;
 pthread_t tid2;

 pthread_mutex_init(&g_mutex,NULL);

 pthread_create(&tid1,NULL,thread_func,NULL);
 pthread_create(&tid2,NULL,thread_func,NULL);

 pthread_join(tid1,NULL);
 pthread_join(tid2,NULL);

 printf("final counter=%d\n",g_counter);
 
 pthread_mutex_destroy(&g_mutex);
 
 return 0;
}
