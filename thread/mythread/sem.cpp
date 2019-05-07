/*POSIX信号量实现一个线程安全的队列 */
#include <iostream>
#include <vector>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

class RingQueue
{
private:
    vector<int> _queue;
    int _capacity;
    int _customer_step;
    int _productor_step;
    sem_t _sem_mutex;
    sem_t _idle_space;//计数还有多少剩余空间
    sem_t _data_space;//计数还有多少空间有数据
private:
    void QueueLock(){
	sem_wait(&_sem_mutex);
    }
    void QueueUnlock(){
	sem_post(&_sem_mutex);
    }
    void CustomerWait(){
	sem_wait(&_data_space);
    }
    void ProductorWait(){
	sem_wait(&_idle_space);
    }
    void CustomerWakeUp(){
	sem_post(&_data_space);
    }
    void ProductorWakeUp(){
	sem_post(&_idle_space);
    }

public:
    RingQueue(int cap = 10)
	: _capacity(cap)
	, _queue(10)
    {
	_productor_step = 0;
	_customer_step = 0;
	sem_init(&_sem_mutex, 0, 1);
	sem_init(&_idle_space, 0, _capacity);
	sem_init(&_data_space, 0, 0);
    }
    
    ~RingQueue()
    {
	sem_destroy(&_sem_mutex);
	sem_destroy(&_idle_space);
	sem_destroy(&_data_space);
    }

    void QueuePush(int data){
	ProductorWait();
	QueueLock();
	_queue[_productor_step] = data;
	_productor_step++;
	//防止存放数据越界
	_productor_step %= _capacity;
	QueueUnlock();
	CustomerWakeUp();
    }

    void QueuePop(int *data){
	CustomerWait();
	QueueLock();
	*data = _queue[_customer_step];
	_customer_step++;
	_customer_step %= _capacity;
	QueueUnlock();
	ProductorWakeUp();
    }
};

void* thr_productor(void *arg){
    RingQueue *q = (RingQueue*) arg;
    int i = 0;
    while(1){
	sleep(1);
	q->QueuePush(i);
	cout << "put data------" << i << endl;
	i++;
    }
    return NULL;
}

void* thr_customer(void* arg){
    RingQueue *q = (RingQueue*) arg;
    while(1){
	sleep(1);
	int data;
	q->QueuePop(&data);
	cout << "get data------" << data << endl;
    }
    return NULL;
}

int main()
{
    int i, ret;
    pthread_t ctid[4], ptid[4];
    RingQueue q;

    for(i = 0; i < 4; i++){
	ret = pthread_create(&ptid[i], NULL, thr_productor, (void*)&q);
	if(ret != 0){
	    cout << "pthread create error!" << endl;
	    return -1;
	}
    }
    for(i = 0; i < 4; i++){
	ret = pthread_create(&ctid[i], NULL, thr_customer, (void*)&q);
	if(ret != 0){
	    cout << "pthread create error!" << endl;
	    return -1;
	}
    }

    for(i = 0; i < 4; i++){
	pthread_join(ptid[i], NULL);
    }
    for(i = 0; i < 4; i++){
	pthread_join(ctid[i], NULL);
    }
    return 0;
}

