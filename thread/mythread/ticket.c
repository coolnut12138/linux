/*卖票程序，体验线程安全，有四个黄牛在抢票，互斥锁的基本使用*/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

//全局变量ticket_count
int ticket_count = 100;
pthread_mutex_t mutex;

void *thr_start(void *arg){
    while(1){
	pthread_mutex_lock(&mutex);
	if(ticket_count > 0){
	    usleep(1000);
	    printf("ticket scalper ---- %d ---%p ---- get ticket:%d\n", (int)arg, pthread_self(), ticket_count);
	    ticket_count--;
	}else{
	    pthread_mutex_unlock(&mutex);
	    return NULL;
	}
	pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    pthread_t tid[4];
    int i, ret;

    pthread_mutex_init(&mutex, NULL);

    for(i = 0; i < 4; i++){
	//pthread_create 成功返回0，失败返回非0
	ret = pthread_create(&tid[i], NULL, thr_start, (void*)i);
	if(ret != 0){
	    printf("pthread create error\n");
	    return -1;
	}
    }

    for(i = 0; i < 4; i++){
	pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}

