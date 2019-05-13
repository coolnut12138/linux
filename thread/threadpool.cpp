/*线程池的实现，线程类，任务类*/

#include <iostream>
#include <queue>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#define MAX_THR 5
#define MAX_QUE 10
using namespace std;

typedef bool (*taskCallback)(int data);

class MyTask
{
public:
    MyTask(){}
    MyTask(int data, taskCallback callback)
	:_data(data)
	, _callback(callback)
    {}
    ~MyTask(){}
    
    void setTask(int data, taskCallback callback) {
	_data = data;
	_callback = callback;
    }

    bool Run() {
	return _callback(_data);
    }
private:
    int _data;
    taskCallback _callback;
};

class ThreadPool
{
public:
    ThreadPool(int max, int cap)
	:_thr_max(max)
	, _capacity(cap)
    {
	_thr_cur = max;
	_quit_flag = false;
	pthread_mutex_init(&_mutex, NULL);
	pthread_cond_init(&_cond_pro, NULL);
	pthread_cond_init(&_cond_con, NULL);
    }
    ~ThreadPool()
    {
	pthread_mutex_destroy(&_mutex);
	pthread_cond_destroy(&_cond_con);
	pthread_cond_destroy(&_cond_pro);
    }

    static void *thr_start(void *arg)
    {
	ThreadPool *p = (ThreadPool*)arg;

	while(1){
	    p->queueLock();
	    while(p->queueIsEmpty()){
		p->consumerWait();
	    }
	    MyTask task;
	    p->queuePop(&task);
	    p->productorWake();
	    p->queueUnLock();
	    task.Run();
	}
	return NULL;
    }
    //为什么不将初始化函数放在构造函数中？	
    //答：首先构造函数没有返回值，不能判断创建线程成功与否，但是有assert可以使用，但是assert如果报错就会退出整个进程，但是创建线程时，如果出错应该再次尝试去创建，不应该退出，所以把有可能出错的初始化尽量不要放在构造函数中。
    bool threadInit()	
    {
	int ret;
	pthread_t tid;
	for(int i = 0; i < _thr_max; i++){
	    ret = pthread_create(&tid, NULL, thr_start, (void*)this);
	    if(ret != 0){
		perror("thread create error\n");
		return false;
	    }
	    pthread_detach(tid);
	}
	return true;
    }

    bool addTask(MyTask &task){
	queueLock();
	if(_quit_flag == true) {
	    queueUnLock();
	    return false;
	}
	while(queueIsFull()){
	    productorWait();
	}
	queuePush(task);
	consumerWake();
	queueUnLock();
	return true;
    }
    void ThreadQuit(){
	queueLock();
	_quit_flag == true;
	queueUnLock();
	while(_thr_cur > 0){
	    consumerWakeAll();
	    usleep(1000);
	}
    }
private:
    bool _quit_flag;	//线程中线程的退出标志
    int _thr_max;   //线程池中最大的线程数量
    int _thr_cur;   //线程池中当前线程数量
    queue<MyTask> _queue;   //任务队列
    int _capacity;  //任务队列的最大结点数
    pthread_mutex_t _mutex;
    pthread_cond_t _cond_pro;
    pthread_cond_t _cond_con;
private:
    void queueLock(){
	pthread_mutex_lock(&_mutex);
    }
    void queueUnLock(){
	pthread_mutex_unlock(&_mutex);
    }
    void productorWait(){
	pthread_cond_wait(&_cond_pro, &_mutex);
    }
    void productorWake(){
	pthread_cond_signal(&_cond_pro);
    }
    void consumerWait(){
	//若用户想要退出则需要选择没有任务的时候
	if(_quit_flag == true){
	    _thr_cur--;
	    pthread_mutex_unlock(&_mutex);
	    printf("thread:%p exit\n", pthread_self());
	    pthread_exit(NULL);
	}
	pthread_cond_wait(&_cond_con, &_mutex);
    }
    void consumerWake(){
	pthread_cond_signal(&_cond_con);
    }
    void consumerWakeAll(){
	pthread_cond_broadcast(&_cond_con);
    }
    bool queueIsEmpty(){
	return _queue.empty();
    }
    bool queueIsFull(){
	return (_capacity == _queue.size());
    }
    void queuePop(MyTask *task){
	*task = _queue.front();
	_queue.pop();
    }
    void queuePush(MyTask &task){
	_queue.push(task);
    }
};

bool taskHandler(int data)
{
    srand(time(NULL));
    int n = rand() % 5;
    printf("thread:%p is sleep %d sec\n", pthread_self(), n);
    sleep(n);
    return true;
}

int main()
{
    ThreadPool p(MAX_THR, MAX_QUE);
    p.threadInit();

    MyTask task[10];
    for(int i = 0; i < 10; i++){
	task[i].setTask(i, taskHandler);
	p.addTask(task[i]);
    }
    p.ThreadQuit();
    return 0;
}
