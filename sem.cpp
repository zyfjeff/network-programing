/*
 =====================================================================================
        Filename:  sem.c
     	Description:  POSIX信号量操作
        Version:  1.0
        Created:  09/02/15 03:10:02
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <unistd.h>
#include <queue>
#include <iostream>

using namespace std;

queue<int> que;
sem_t var_sem; //信号量，线程间同步
pthread_mutex_t lock;

void *producer_process(void* data)
{
	int count = 1;
	while(1)
	{
		pthread_mutex_lock(&lock);
		que.push(count++);
		cout << "producer ........." << endl;
		sem_post(&var_sem);
		pthread_mutex_unlock(&lock);
		sleep(2);
	}
	
}
void *coustom_process(void* data)
{
	while(1)
	{
		sem_wait(&var_sem);
		pthread_mutex_lock(&lock);
		cout << "consumer ........."<< que.front() << endl;
		que.pop();
		pthread_mutex_unlock(&lock);
	}
}


int main()
{
	int ret = sem_init(&var_sem,0,0); //信号初始化
	pthread_mutex_init(&lock,NULL);
	assert(ret == 0);
	pthread_t producer; //生产者线程
	pthread_t coustom; //消费者线程
	ret = pthread_create(&producer,NULL,producer_process,NULL);
	assert(ret == 0);
	ret = pthread_create(&coustom,NULL,coustom_process,NULL);
	assert(ret == 0);
	pthread_join(producer,NULL);
	pthread_join(coustom,NULL);
}
