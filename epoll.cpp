/*
 *	epoll模型测试
 *
 */
#include <sys/epoll.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAXLINE 10
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 80
#define INFTIM 1000

void setnonblocking(int sock);

//设置描述符为阻塞
void setnonblocking(int sock)
{
	int opts;
	opts = fcntl(sock,F_GETFL);
	if(opts < 0)
	{
		perror("fcntl(sock,F_GETFL)");
		exit(1);
	}
	opts = opts|O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts) < 0)
	{
		perror("fcntl(sock,F_SETFL),opts");
		exit(1);
	}
}


int main()
{
	int i,maxi,listenfd,connfd,sockfd,epfd,nfds;
	ssize_t n;
	char line[MAXLINE];
	socklen_t clilen;

	//声明epoll_event结构体的变量,ev用户注册事件，数组用于回传要处理的事件
	struct epoll_event ev,events[20];

	//生成用户处理accept的epoll的文件描述符
	epfd = epoll_create(256);
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	//监听套接字
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	
	//把socket设置为非阻塞
	setnonblocking(listenfd);
	//设置要处理的事件相关的文件描述符
	ev.data.fd = listenfd;
	ev.events = EPOLLIN|EPOLLET;//sodcket可读或者是有事件发生
	
	//注册epoll事件
	epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

	//开始监听
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	
	const char *local_addr = "192.168.10.141";
	inet_aton(local_addr,&(serveraddr.sin_addr)); //htons(SERV_PORT) //转换ip地址
	serveraddr.sin_port = htons(SERV_PORT); //转换port
	//套接字绑定
	bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	//开始监听
	listen(listenfd,LISTENQ);

	maxi = 0;
	for(;;)
	{
		//等待epoll事件的发生,返回就绪的文件描述符,最多返回20个,500是超时值
		nfds = epoll_wait(epfd,events,20,500);
		//处理所发生的所有事件,遍历所有发生事件的描述符
	    for(i = 0;i < nfds;++i)
	    {
		if(events[i].data.fd == listenfd) //判断是否是监听套接字
		{
			connfd = accept(listenfd,(struct sockaddr*)&clientaddr,&clilen);
			if(connfd < 0)
			{
				perror("connfd < 0");
				exit(1);
			}
			setnonblocking(connfd);
			char *str = inet_ntoa(clientaddr.sin_addr);
			std::cout << "connect from: "<< str << std::endl;
			//设置用于读操作的文件描述符
			ev.data.fd = connfd;
			//设置用于注册得到读操作事件
			ev.events = EPOLLIN|EPOLLET;
			//注册ev
			epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
		}
		else if(events[i].events & EPOLLIN) //是数据读事件
		{
			if(( sockfd = events[i].data.fd) <0 )continue; //判断fd是否合法
			if((n = read(sockfd,line,MAXLINE)) < 0) //读取数据
			{
				if(errno == ECONNRESET)  //连接被重置了，则关闭socket
				{
					close(sockfd);
					events[i].data.fd = -1;
				}
				else
					std::cout << "readline error" << std::endl; //其他情况输出错误
			}
			else if(n == 0) //没有数据可读
			{
				close(sockfd);
				events[i].data.fd = -1;
			}
			ev.data.fd = sockfd;
			ev.events = EPOLLOUT|EPOLLET; //注册为写事件
			epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
		}
		else if(events[i].events & EPOLLOUT) // 是数据写事件
		{
			sockfd = events[i].data.fd; //获取sockfd
			write(sockfd,line,n); //写入line数据,
			ev.data.fd = sockfd;
			ev.events = EPOLLIN|EPOLLET;
			epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev); //注册为读事件
		}
	    }

	}
}
