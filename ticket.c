/*卖票程序，体验线程安全，有四个黄牛在抢票，互斥锁的基本使用*/
//互斥：让每一个人抢到一张真票
//同步：让每一个人都能抢到票

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

//全局变量ticket_count
int ticket_count = 100;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex;

void *thr_start(void *arg){
    while(1){
	pthread_mutex_lock(&mutex);
	if(ticket_count > 0){
	    usleep(10000);
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
    
    //1、在创建线程之前就要初始化互斥锁
    pthread_mutex_init(&mutex, NULL);

    for(i = 0; i < 4; i++){
	//pthread_create 成功返回0，失败返回非0
	ret = pthread_create(&tid[i], NULL, thr_start, (void*)i);  //如果这块 i 传的是地址，那么意味着几个线程访问的是同一块地址空间，拿到的数值完全相同,虽然线程不同但是拿到的数据是一样的，比如运行结果虽然都是线程0拿到票，但是实际上这些线程0可能是不同的线程，只不过名字相同罢了。
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

