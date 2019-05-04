/*实现生产者与消费者模型*/

#include <iostream>
#include <queue>
#include <string>
#include <pthread.h>
#include <unistd.h>
using namespace std;

class BlockQueue{
public:
    BlockQueue(int cap = 10)
	:_capacity(cap)
    {
	pthread_mutex_init(&_mutex, NULL);
	pthread_cond_init(&_productor, NULL);
	pthread_cond_init(&_customer, NULL);
    }
    ~BlockQueue()
    {
	pthread_mutex_destroy(&_mutex);
	pthread_cond_destroy(&_productor);
	pthread_cond_destroy(&_customer);
    }

    void QueuePush(int val)
    {
	QueueLock();
	while(QueueIsFull()){
	    ProductorWait();
	}
	_queue.push(val);
	QueueUnlock();
	CustomerWakeUp();
    }
    void QueuePop(int *val)
    {
	QueueLock();
	while(QueueIsEmpty()){
	    CustomerWait();
	}
	*val = _queue.front();
	_queue.pop();
	QueueUnlock();
	ProductorWakeUp();
    }
private:
    queue<int> _queue;
    int _capacity;
    pthread_mutex_t _mutex;
    pthread_cond_t _productor;
    pthread_cond_t _customer;
private:
    void QueueLock(){
	pthread_mutex_lock(&_mutex);
    }
    void QueueUnlock(){
	pthread_mutex_unlock(&_mutex);
    }
    void CustomerWait(){
	pthread_cond_wait(&_customer, &_mutex);
    }
    void CustomerWakeUp(){
	pthread_cond_signal(&_customer);
    }
    void ProductorWait(){
	pthread_cond_wait(&_productor, &_mutex);
    }
    void ProductorWakeUp(){
	pthread_cond_signal(&_productor);
    }
    bool QueueIsEmpty(){
	return _queue.empty();
    }
    bool QueueIsFull(){
	return (_queue.size() == _capacity);
    }
};

void *customer(void *arg)
{
    BlockQueue *q = (BlockQueue*)arg;
    while(1){
	int data;
	q->QueuePop(&data);
	cout << "get data:" << data << endl;
    }
    return NULL;
}

void *productor(void *arg)
{
    BlockQueue *q = (BlockQueue*)arg;
    int i = 0;
    while(1){
	q->QueuePush(i);
	cout << "---put data:" << i << endl;
	i++;
    }
    return NULL;
}

int main()
{
    BlockQueue q;
    pthread_t c_tid[4], p_tid[4];
    for(int i = 0; i < 4; i++){
	if(pthread_create(&p_tid[i], NULL, productor, (void*)&q) != 0){
	    cout << "create thread error" << endl;
	    return -1;
	}
    }
    for(int i = 0; i < 4; i++){
	if(pthread_create(&c_tid[i], NULL, customer, (void*)&q) != 0){
	    cout << "create thread error" << endl;
	    return -1;
	}
    }

    for(int i = 0; i < 4; i++){
	pthread_join(c_tid[i], NULL);    
    }
    for(int i = 0; i < 4; i++){
	pthread_join(p_tid[i], NULL);    
    }
    return 0;
}

