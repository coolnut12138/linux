/*创建线程 以及进程终止*/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_start(void *arg)
{
    pthread_t tid = pthread_self();
    while(1){
	//注意线程之间没有父子关系
	printf("child thread-------%d---tid:%p\n", arg, tid);
	sleep(1);
	//exit 退出的是整个进程（所有线程都会退出）
	//exit(0);
	
	//不能让整个进程退出，所以用到这个函数来退出线程
	//void pthread_exit(void *retval);
	//  退出调用线程，并且返回retval
	//  线程退出的返回值，不仅可以判断终止场景，并且可以获取任务处理结果。那么其他进程就可以访问到这块地址，访问到这块内存，进而获取这个线程的状态。
	//pthread_exit("nihao~~");
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thr_start, (void*)999);
    if(ret != 0){
	printf("pthread create error\n");
	return -1;
    }

    sleep(1);
    //int pthread_cancel(pthread_t thread);
    //	取消指定线程---让指定线程退出
    //	thread：线程id
    pthread_cancel(tid);
    while(1){
	//主线程
	printf("main thread--------child tid:%p, mypid:%d\n", tid, getpid());
	sleep(1);
	//pthread_exit("nihao~~");
	//return 0;
    }
    return 0;
}
