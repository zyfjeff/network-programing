/*	
 *	线程的基本使用
 *
 *
 */ 
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


/*
 *	线程的一些要素
 *	pthread_t 线程id
 *	pthread_attr_t 线程属性
 *	线程函数的原型 void *func(void *data);
 *	pthread_join 等待一个给定的线程终止,并且可以收集返回值
 *	pthread_self 获取线程自身ID
 *	pthread_deach 把指定线程设置为脱离状态
 *	pthread_exit 让线程终止
 *	线程终止的其他两个办法:
 *	1.启动的线程函数可以返回return
 *	2.进程的main函数返回,或者是任何线程调用exit，整个进程就终止了。
 *	
 *	线程安全函数:
 *		后缀名带_r
 *	线程特定数据:
 *
 */ 

void * func(void *data)
{
	printf("Hello World\n");
	exit(1);
}

int main()
{
	pthread_t tid;
//线程达到最大数目返回EAGAIN
//ptthread系列函数出错不设置errno
	pthread_create(&tid,NULL,func,NULL);	
	printf("Hello Worldaaa\n");
	while(1);
}
