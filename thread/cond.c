/*条件变量的基本操作和认识*/
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int have_noodle = 0;
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//使用一个条件变量可能会导致唤醒紊乱，所以用两个条件变量
pthread_cond_t chimian; //吃面的人等在这
pthread_cond_t zuomian;	//做面的人等在这
pthread_mutex_t mutex;
void *thr_sale(void *arg)
{
    while(1){
	pthread_mutex_lock(&mutex);
	//有面没人吃则等待
	while(have_noodle != 0){
	    //sleep(1);
	    
	    //int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
	    //	cond：条件变量
	    //	mutex：互斥锁
	    //条件变量为什么需要锁？
	    //答：此处的have_noodle就是我们的判断条件，这是一个全局变量，是临界资源。它的操作就应该受保护，条件变量的使用势必会用到外部条件的判断，外部条件的判断通常是全局数据，所以需要加锁。
	    pthread_cond_wait(&zuomian, &mutex);
	}
	printf("sale----make noodles!!\n");
	have_noodle = 1;
	//int pthread_cond_broadcast(pthread_cond_t *cond);
	//  唤醒所有等待线程
	//int pthread_cond_signal(pthread_cond_t *cond);
	//  唤醒至少一个等待的线程
	pthread_cond_signal(&chimian);
	pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *thr_buy(void *arg)
{
    while(1){
	pthread_mutex_lock(&mutex);
	while(have_noodle == 0){
	    pthread_cond_wait(&chimian, &mutex);
	    //sleep(1);
	}
	printf("buy --- delicious!!\n");
	have_noodle = 0;
	//吃完面就唤醒做面的人
	pthread_cond_signal(&zuomian);
	pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    
    int ret, i;

    pthread_cond_init(&chimian, NULL);
    pthread_cond_init(&zuomian, NULL);
    pthread_mutex_init(&mutex, NULL);
    for(i = 0; i < 2; i++){
	ret = pthread_create(&tid1, NULL, thr_sale, NULL);
	if(ret != 0) {
	    printf("thread create error\n");
	}
    }

    for(i = 0; i < 4; i++){
	ret = pthread_create(&tid2, NULL, thr_buy, NULL);
	if(ret != 0) {
	    printf("thread create error\n");
	}
    }
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_cond_destroy(&chimian);
    pthread_cond_destroy(&zuomian);
    pthread_mutex_destroy(&mutex);
    return 0;
}
