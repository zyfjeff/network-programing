/*
 =====================================================================================
        Filename:  processpool_demo.cpp
     	Description:  进程池的简单封装
        Version:  1.0
        Created:  15/02/15 23:31:17
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <iostream>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

class procespool
{
private://实现单例模式
	procespool():m_idx(-1){
		for(int i  = 0;i < 5;i++)
		{
			pid_t pid = fork();
			assert(pid != -1);
			if (pid > 0) {
				continue;
			}else if ( pid == 0) {
				m_idx = i;
				break;
			}
		}		
	};

public:
	static procespool* create(){
		if(!m_instance)
			m_instance = new procespool();
		return m_instance;	
	}
	void run()
	{
		if (m_idx != -1) {
			run_child();
			return;
		}
		run_parent();
	}
	void run_parent()
	{
		while(1)
		{
			cout << "I am father process" << m_idx << endl;
			sleep(10);
		}
	
	}
	void run_child()
	{
		while(1)
		{	
			cout << "I am new child"<<m_idx << endl;
			cout << "I am pid"<<getpid() << endl;
			sleep(5);
		}
	}
private:
	int m_idx;
	pid_t masterpid;
	static procespool *m_instance;

};

procespool* procespool::m_instance = NULL;

int main()
{
	procespool *p = procespool::create();
	p->run();
}
