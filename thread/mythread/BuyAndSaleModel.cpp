/*实现生产者与消费者模型*/
#include <iostream>
#include <queue>
#include <pthread.h>
using namespace std;

class TaskQueue
{
    public:
	TaskQueue(int cap = 10)
	    :_capacity(cap)
	{
	    pthread_mutex_init(&_mutex, NULL);
	    pthread_cond_init(&_productor, NULL);
	    pthread_cond_init(&_customer, NULL);
	}
	
	~TaskQueue()
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
	    QueueUnLock();
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
	    QueueUnLock();
	    ProductorWakeUp();
	}
    private:
	queue<int> _queue;
	int _capacity;
	pthread_mutex_t _mutex;
	pthread_cond_t _productor;
	pthread_cond_t _customer; 
    private:
	void QueueLock()
	{
	    pthread_mutex_lock(&_mutex);
	}
	void QueueUnLock()
	{
	    pthread_mutex_unlock(&_mutex);
	}
	void CustomerWait()
	{
	    pthread_cond_wait(&_customer, &_mutex);
	}
	void CustomerWakeUp()
	{
	    pthread_cond_signal(&_customer);
	}
	void ProductorWait()
	{
	    pthread_cond_wait(&_productor, &_mutex);
	}
	void ProductorWakeUp()
	{
	    pthread_cond_signal(&_productor);
	}
	bool QueueIsFull()
	{
	    return _queue.size() == _capacity;
	}
	bool QueueIsEmpty()
	{
	    return _queue.empty();
	}
};

void* productor(void* arg)
{
    TaskQueue* q = (TaskQueue*)arg;
    int i = 0; 
    while(1){
	q->QueuePush(i);
	cout << "producting data -------------- :" << i << endl;
	i++;
    }
    return NULL;
}

void* customer(void* arg)
{
    TaskQueue* q = (TaskQueue*)arg;
    while(1){
	int data;
	q->QueuePop(&data);
	cout << "Geting data&&&&&&&&&&&&&&&&&&&&&&&&&&& :" << data << endl;
    }
    return NULL;
}

int main()
{
    TaskQueue tq;
    pthread_t c_tid[4];
    pthread_t p_tid[4];
    for(int i = 0; i < 4; i++){
	int ret = pthread_create(&p_tid[i], NULL, productor, (void*)&tq);
	if(ret != 0){
	    cout << "pthread_create error" << endl;
	    return -1;
	}
    }
    for(int i = 0; i < 4; i++){
	int ret = pthread_create(&c_tid[i], NULL, customer, (void*)&tq);
	if(ret != 0){
	    cout << "pthread_create error" << endl;
	    return -1;
	}
    }

    for(int i = 0; i < 4; i++){
	pthread_join(c_tid[i], NULL);
	pthread_join(p_tid[i], NULL);
    }
    return 0;
}
