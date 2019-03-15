#include <stdio.h>
#include <string.h>
#include <pthread.h>

/*
    线程私有数据：key变量可以供所有线程进行赋值和值访问，
    并各线程之间互不影响，当每个线程退出时，pthread_key_create第二个参数
    函数指针中以key作为参数传递，主线程退出时对该key进行释放
*/
pthread_key_t key;
void echomsg(char* t)
{
    printf("destructor excuted in thread %d,param=%s\n",pthread_self(),t);
}
void * child1(void *arg)
{
    char szBuff[1024];
    memset(szBuff, 0x00, sizeof(szBuff));
    memcpy(szBuff, "child1", sizeof(szBuff)-1);
    printf("child1 szBuff=%s\n",szBuff);
    pthread_setspecific(key,(void *)szBuff);
    sleep(2);
    printf("child1 id=%d, key=%s\n",pthread_self(), pthread_getspecific(key));
    sleep(5);
}
void * child2(void *arg)
{
    char szBuff[1024];
    memset(szBuff, 0x00, sizeof(szBuff));
    memcpy(szBuff, "child2", sizeof(szBuff)-1);
    printf("child2 szBuff=%s\n",szBuff);
    pthread_setspecific(key,(void *)szBuff);
    sleep(2);
    printf("child2 id=%d, key=%s\n",pthread_self(), pthread_getspecific(key));
    sleep(5);
}
int main(void)
{
    pthread_t tid1,tid2;
    pthread_key_create(&key,echomsg);
    pthread_create(&tid1,NULL,child1,NULL);
    pthread_create(&tid2,NULL,child2,NULL);
    sleep(10);
    pthread_key_delete(key);
    printf("main thread exit\n");
    return 0;
}
