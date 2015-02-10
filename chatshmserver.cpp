/*
 =====================================================================================
        Filename:  chatshmserver.c
     	Description:  基于共享内存的聊天服务器
        Version:  1.0
        Created:  09/02/15 11:30:30
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

//用户最大数量，缓存大小，描述符最大值，进程号最大值，事件集合最大值
#define USER_LIMIT 15
#define BUFFER_SIZE 1024
#define FD_LIMIT 65535
#define MAX_EVENT_NUMBER 1024
#define PROCESS_LIMIT 65535


//一个客户连接的数据
struct client_data
{
	sockaddr_in address;
	int connfd;
	pid_t pid;
	int pipefd[2];
};

//共享内存的标识
static const char* shm_name = "/my_shm";
// 用户统一信号事件源
int sig_pipefd[2];

int epollfd;
int listenfd;
int shmfd;

char *share_mem = 0;
client_data *users = 0;
int * sub_process = 0;
int user_count = 0;
bool stop_child = false;


//设置fd为非阻塞
int setnonblocking(int fd)
{
	int old_option = fcntl(fd,F_GETFL);
	int new_option = old_option|O_NONBLOCK;
	fcntl(fd,F_SETFL,new_option);
	return old_option;
}


//添加fd到epoll集合中
void addfd(int epollfd,int fd)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN|EPOLLET; //可读，边缘触发模式
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
	setnonblocking(fd);
}

//信号处理函数，多线程，多进程环境中为了可重入要保存errno
void sig_handler(int sig)
{
	int save_errno = errno;
	int msg = sig;
	send(sig_pipefd[1],(char *)&msg,1,0);
	errno = save_errno;	
}

//添加信号,设置自重启
void add_sig(int sig,void(*handler)(int),bool restart = true)
{
	struct sigaction sa;
	bzero(&sa,sizeof(struct sigaction));
	sa.sa_handler = handler;
	if(restart)
	{
		sa.sa_flags |= SA_RESTART;
	}
	sigfillset(&sa.sa_mask);//屏蔽所有信号
	assert(sigaction(sig,&sa,NULL) != -1);
}

//释放资源
void del_resource()
{
	close(sig_pipefd[0]);
	close(sig_pipefd[1]);
	close(listenfd);
	close(epollfd);
	shm_unlink(shm_name);
	delete []users;
	delete sub_process;
}

//子进程关闭的标志位
void child_term_handler(int sig)
{
	stop_child = true;
}


//子进程运行的函数
int run_child(int idx,client_data* user,char* share_mem)
{
	epoll_event events[MAX_EVENT_NUMBER];
	int child_epollfd = epoll_create(5);
	assert(child_epollfd != -1);
	int connfd = user[idx].connfd;
	addfd(child_epollfd,connfd);
	int pipefd = user[idx].pipefd[1];
	addfd(child_epollfd,pipefd);
	int ret;
	add_sig(SIGTERM,child_term_handler,false);
	while(!stop_child)
	{
		int number = epoll_wait(child_epollfd,events,MAX_EVENT_NUMBER,-1);
		if((number < 0) && (errno != EINTR))//epoll错误,跳出循环
		{
			printf("epoll failure\n");
			break;
		}
		for(int i = 0;i < number;i++)//客户连接有数据到来,开始读取数据
		{
			int sockfd = events[i].data.fd; //获取客户连接的fd,判断是不是本进程负责的客户
			//判断数据是否是可读
			if((sockfd == connfd) && (events[i].events &EPOLLIN))
			{
				//初始化共享内存
				bzero(share_mem+idx*BUFFER_SIZE,BUFFER_SIZE);
				//读取数据
				ret = recv(connfd,share_mem+idx*BUFFER_SIZE,BUFFER_SIZE-1,0);
				if(ret < 0)
				{
					//非阻塞返回的错误码，不是一个错误只是无数据可读
					if(errno != EAGAIN)
					{
						stop_child = true;
					}
				}else if(ret == 0) //连接关闭
				{
					stop_child = false;
				}else 
					send(pipefd,(char*)&idx,sizeof(idx),0); //通知主进程处理数据
			}else if((sockfd == pipefd) && (events[i].events & EPOLLIN))
			{
				int client = 0;
				ret = recv(sockfd,(char *)&client,sizeof(client),0);
				if(ret < 0)
				{
					if(errno != EAGAIN)
					{
						stop_child = true;
					}
				}else if(ret == 0)
					stop_child = false;
				else
					send(connfd,share_mem+client*BUFFER_SIZE,BUFFER_SIZE,0);
			}else
				continue;
		}
	}
	close(connfd);
	close(pipefd);
	close(child_epollfd);
	return 0;
}

int main(int argc,char* argv[])
{
	if(argc <= 2)
	{
		printf("usage: %s ip_address port_number\n",basename(argv[0]));
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi(argv[2]);
	int ret = 0;
	struct sockaddr_in address;
	bzero(&address,sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET,ip,&address.sin_addr);
	address.sin_port = htons(port);
	listenfd = socket(PF_INET,SOCK_STREAM,0);
	assert(listenfd >= 0);
	ret = bind(listenfd,(struct sockaddr*)&address,sizeof(address));
	assert(ret != -1);
	ret = listen(listenfd,5);
	assert(ret != -1);
	//上面都是socket的基本过程
	user_count = 0;
	users = new client_data[USER_LIMIT+1];
	sub_process  = new int[PROCESS_LIMIT+1];
	for(int i = 0;i < PROCESS_LIMIT;++i)
	{
		sub_process[i] = -1;
	}
	
	epoll_event events[MAX_EVENT_NUMBER];
	epollfd = epoll_create(5);
	assert(epollfd != -1);
	addfd(epollfd,listenfd);
	

	ret = socketpair(PF_UNIX,SOCK_STREAM,0,sig_pipefd);
	assert(ret != -1);
	setnonblocking(sig_pipefd[1]);
	setnonblocking(sig_pipefd[0]);

	add_sig(SIGCHLD,sig_handler);
	add_sig(SIGTERM,sig_handler);
	add_sig(SIGINT,sig_handler);
	add_sig(SIGPIPE,SIG_IGN);
	bool stop_server = false;
	bool terminate = false;

	//创建共享内存，作为读缓存
	
	shmfd  = shm_open(shm_name,O_CREAT|O_RDWR,0666);
	assert(shmfd != -1);
	
	ret = ftruncate(shmfd,USER_LIMIT*BUFFER_SIZE);
	assert(ret != -1);
	
	share_mem = (char*)mmap(NULL,USER_LIMIT*BUFFER_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,shmfd,0);
	assert(share_mem != MAP_FAILED);
	close(shmfd);

	while(!stop_server)
	{

		int number = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
		if((number < 0) && (errno != EINTR))//不需要人为重启epoll_wait,已经设置了restart了
		{
			perror("epoll failure");
			break;
		}
		for(int i = 0;i < number;i++) //遍历返回的所有文件描述符
		{
			int sockfd = events[i].data.fd; //首先获取fd
			if(sockfd == listenfd) //判断是否是监听描述符
			{
				struct sockaddr_in client_address;
				socklen_t client_addrlength = sizeof(client_address);
				int connfd = accept(listenfd,(struct sockaddr*)&client_address,&client_addrlength);
				if(connfd < 0)
				{
					printf("errno is:%d\n",errno);
					continue;
				}//判断用户数量
				if(user_count >= USER_LIMIT)
				{
					const char* info = "too many user";
					printf("%s",info);
					send(connfd,info,strlen(info),0);
					close(connfd);
					continue;
				}
				//保存第user_count个客户的相关数据
				users[user_count].address = client_address;
				users[user_count].connfd = connfd;
				
				//主进程和子进程建立管道，传递必要的数据
				ret = socketpair(PF_UNIX,SOCK_STREAM,0,users[user_count].pipefd);
				assert(ret != -1);
				pid_t pid = fork();
				if(pid < 0)
				{
					close(connfd);
					continue;
				}
				else if(pid == 0) //子进程
				{
					close(epollfd);
					close(listenfd);
					close(users[user_count].pipefd[0]);
					close(sig_pipefd[0]);
					close(sig_pipefd[1]);
					run_child(user_count,users,share_mem);
					munmap((void*)share_mem,USER_LIMIT*BUFFER_SIZE);
					exit(0);
				}
				else //父进程
				{
					close(connfd);
					close(users[user_count].pipefd[1]);
					addfd(epollfd,users[user_count].pipefd[0]);
					users[user_count].pid = pid;
					//建立pid和user_count的关系
					sub_process[pid] = user_count;
					user_count++;
				}
			}//end if
				//统一事件源了
			else if((sockfd == sig_pipefd[0]) && (events[i].events & EPOLLIN)) //信号事件可读
			{
				int sig;
				char signals[1024];
				ret = recv(sig_pipefd[0],signals,sizeof(signals),0);
				if(-1 == ret)
				{
					continue;
				}
				else if(ret == 0)
				{
					continue;
				}
				else
				{
					for(int i = 0;i < ret;++i)
					{
						switch(signals[i])
						{
							case SIGCHLD:
							{
								pid_t pid;
								int stat;
								while((pid = waitpid(-1,&stat,WNOHANG)) > 0)
								{
									int del_user = sub_process[pid];
									sub_process[pid] = -1;
									if((del_user < 0) || (del_user > USER_LIMIT))
									{
										continue;
									}
									//清除第del_user个客户的连接数据
									epoll_ctl(epollfd,EPOLL_CTL_DEL,users[del_user].pipefd[0],0);
									close(users[del_user].pipefd[0]);											 users[del_user] = users[--user_count];
									sub_process[users[del_user].pid] = del_user;
								}//end while
								if(terminate && user_count == 0)
								{
									stop_server = true;		
								}
								break;
							}//end case SIGCHLD
							case SIGTERM:
							case SIGINT:
							{
							//结束服务器程序
								printf("kill all the child now\n");
								if(user_count == 0)
								{
									stop_server = true;
									break;
								}
								for(int i = 0;i< user_count;++i)
								{
									int pid = users[i].pid;
									kill(pid,SIGTERM);
								}
								terminate = true;//关闭标志
								break;
							}
							default:
								break;
						}//end switch
					}//end for
				} //end else
			}// end else if

			//有数据可读
			else if(events[i].events & EPOLLIN)
			{
				int child = 0;
				ret = recv(sockfd,(char*)&child,sizeof(child),0);
				printf("read data from child accross pipe\n");
				if(ret == -1)
				{
					continue;	
				}
				else if(ret == 0)
				{
					continue;
				}
				else
				{
					for(int j = 0;j < user_count;++j)
					{
						//广播消息
						if(users[j].pipefd[0] != sockfd)
						{
							printf("send data to child accross pipe \n");
							send(users[j].pipefd[0],(char*)&child,sizeof(child),0);
						}
					}
				}
			}//end else if
		}//end for

	}//end while
	del_resource();
	return 0;
} // end main

