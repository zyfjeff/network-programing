/*
 =====================================================================================
        Filename:  thread_sig.cpp
     	Description:  线程处理信号
        Version:  1.0
        Created:  10/02/15 16:24:09
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define handle_error_en(en,msg)\
	do{errno = en;perror(msg);exit(EXIT_FAILURE);}while(0)

static void *sig_thread(void *arg)
{
	sigset_t *set = (sigset_t *)arg;
	int s,sig;
	for(;;)
	{
		s = sigwait(set,&sig);
		if(s != 0)
			handle_error_en(s,"pthread_sigmask");
		printf("signal handlinhg thread got signal %d\n",sig);
	}
}

int main(int argc,char*argv[])
{
	pthread_t thread;
	sigset_t set;
	int s;
	sigemptyset(&set);
	sigaddset(&set,SIGQUIT);
	sigaddset(&set,SIGUSR1);
	s = pthread_sigmask(SIG_BLOCK,&set,NULL);
	if(s != 0)
		handle_error_en(s,"pthread_sigmask");
	s = pthread_create(&thread,NULL,&sig_thread,(void*)&set);
	if(s != 0)
		handle_error_en(s,"pthread_create");
	pause();
}
