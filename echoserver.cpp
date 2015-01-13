/*
 *	回射服务器端
 *
 */
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

#define ERR_EXIT(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)

void print_exit(int status)
{
    if(WIFEXITED(status)) //是正常退出
        cout << "normal termination,exit status" << WEXITSTATUS(status) << endl;
    else if(WIFSIGNALED(status)) //子进程异常终止
    {
     	cout << "abnormal termination,signal number" <<WTERMSIG(status)<< endl;
#ifdef WCOREDUMP
     if(WCOREDUMP(status))  //判断该信号时候会产生coredump文件
        cout << "core file generated" << endl;  
#else
#endif
     }
     else if(WIFSTOPPED(status)) //子进程暂停
         cout << "child stopped,signal number" << WSTOPSIG(status) << endl;
}

int childfunc(int fd)
{
	while(1)
	{
		char line[1024];
                bzero(line,sizeof(line));
                int ret =  recv(fd,line,sizeof(line),0);
		if(ret > 0){
               	 	cout << line << endl;
		}else if(ret < 0){
			cout << "recv error" << endl;
		}else if(ret == 0){
			cout << "client close" << endl;
			break;
		}
                ret = send(fd,line,sizeof(line),0);
		if(ret == -1)
			perror("write:");
                bzero(line,sizeof(line));
	}
	close(fd);
}

int MakeServer(int backlog)
{
	int ret;
	int sockfd = socket(PF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
		ERR_EXIT("create socket");
	struct sockaddr_in addr;
	addr.sin_port = htons(81);
	addr.sin_family = AF_INET;
	if((ret = inet_pton(PF_INET,"0.0.0.0",&(addr.sin_addr))) < 0)
		ERR_EXIT("convert to ip");
	int value = 1;
	if((ret = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value))) < 0)
		ERR_EXIT("set socket");
	if((ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))) < 0)
		ERR_EXIT("bind socket");
	if((ret = listen(sockfd,backlog)) < 0)
		ERR_EXIT("listen socket");
	return sockfd;
}

void handle(int signum)
{
	pid_t pid;
	int status;
	while((pid = waitpid(-1,&status,WNOHANG)) > 0)
	{
		print_exit(status);
	}
}


int main()
{
	int sockfd = MakeServer(10);
	char ipbuf[1024];
	signal(SIGCHLD,handle);
	struct sockaddr_in caddr; //客户端连接过来的源IP
	socklen_t addrlen = sizeof(struct sockaddr_in);
	while(1){
		//int fd = accept(sockfd,(struct sockaddr *)&caddr,&addrlen);
		int fd = accept(sockfd,NULL,NULL);
		if(fd == -1)
			ERR_EXIT("accept client");
		else
		    {
			getpeername(fd,(struct sockaddr *)&caddr,&addrlen);
			cout << inet_ntoa(caddr.sin_addr) << endl;
			pid_t pid;
			if((pid = fork()) == -1)
				cout << "fork error" << endl;
			else if(pid > 0)
				continue;
			else if(pid == 0)
			{
				childfunc(fd);
				exit(EXIT_SUCCESS);
			}
		//inet_ntop(PF_INET,&caddr,ipbuf,INET_ADDRSTRLEN);
		//cout << ipbuf << endl;
		    }
	}
	close(sockfd);
}

