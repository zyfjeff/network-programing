#include <sys/wait.h>
#include <sys/types.h>


/*
 *	标准的信号处理函数
 *
 */ 

void sig_child(int signum)
{
	pid_t pid;
	int stat;
	while((pid = waitpid(-1,&stat,WNOHANG)) > 0)
	{
			//deal_with someting
	}
	return;
}
int main()
{	
	
}

	
