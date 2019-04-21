/*线程等待*/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_start(void *arg)
{
    sleep(3);
    return "abc";
}

int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thr_start, NULL);
    if(ret != 0){
	printf("pthread create errno\n");
	return -1;
    }
    //int pthread_join(pthread_t thread, void **retval);
    //	功能：阻塞等待线程退出
    //	thread: 等待的线程ID
    //	retval：用户获取线程的返回退出值
    char *retval;
    pthread_join(tid, (void**)&retval);
    printf("retval:%s\n", retval);
    //char **retval = NULL;   这是不合理的，char**初始化为NULL，那么char*就没有空间，找不到char*
    //pthread_join(tid, (void**)retval);
    //printf("retval:%s\n", *retval);
    while(1){
	sleep(1);
    }
    return 0;
}
