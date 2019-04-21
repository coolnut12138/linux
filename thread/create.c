/*创建线程*/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_start(void *arg)
{
    //pthread_t pthread_self(void);
    //	返回调用线程的线程ID
    pthread_t tid = pthread_self();
    while(1){
	//注意线程之间没有父子关系
	printf("child thread-------%d---tid:%p\n", arg, tid);
	sleep(1);
	exit(0);
    }
    return NULL;
}

int main()
{
    //int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
    //	    thread: 用于保存返回的线程ID
    //	    attr：  线程属性，通常置空
    //	    start_routine：线程入口函数
    //	    arg：   给线程传入的参数
    //	    返回值：成功：0	失败：errno
    //	    使用线程创建函数要在makefile中添加-lpthread
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thr_start, (void*)999);
    if(ret != 0){
	printf("pthread create error\n");
	return -1;
    }

    while(1){
	//主线程
	printf("main thread--------child tid:%p, mypid:%d\n", tid, getpid());
	sleep(1);
    }
    return 0;
}
