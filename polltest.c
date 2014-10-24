/*
 * =====================================================================================
 *
 *       Filename:  polltest.c
 *
 *    Description:  poll模型的简单测试和学习
 *
 *        Version:  1.0
 *        Created:  2014年10月23日 18时21分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jeff (jeff), zyfforlinux@163.com
 *        Company:  xupt
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <limits.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/stropts.h>

#define MAXLINE 4096
#define OPEN_MAX 4096
int main()
{
	struct sockaddr_in clientaddr,addr;
	socklen_t clilen;
	int readyfd,maxi,connfd,sockfd;
	int ret,i,n;
	char buf[MAXLINE];
	struct pollfd clientfd[OPEN_MAX];//定义pollfd结构体
	//create socket
	int listenfd = socket(AF_INET,SOCK_STREAM,0); 
	if(listenfd == -1) perror("create socket error"),exit(-1);
	
	//bind socket
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenfd,(struct sockaddr*)&addr,sizeof(addr));
	
	//listen in the sockte
	ret = listen(listenfd,5);
	if(ret == -1) perror("listenfd error"),exit(-1);

	clientfd[0].fd = listenfd;
	clientfd[0].events = POLLRDNORM;
	for(i = 1 ;i < OPEN_MAX;i++)
	{
		clientfd[i].fd = -1;
	}
	maxi = 0; //记录fd最大索引
	for(;;){
		readyfd = poll(clientfd,maxi+1,-1); //开始监听，返回最大文件描述符
		if(clientfd[0].revents & POLLRDNORM){
			clilen = sizeof(clientaddr);
			connfd = accept(listenfd,(struct sockaddr*)&clientaddr,&clilen);
			//打印客户端ip
			printf("client ip:%s\n",inet_ntoa(clientaddr.sin_addr));
			for(i = 1;i<OPEN_MAX;i++){
				if(clientfd[i].fd < 0){
					clientfd[i].fd = connfd;
					break;
				}
			}
			if(i == OPEN_MAX)
				perror("too many clients");
			clientfd[i].events = POLLRDNORM;
			if(i > maxi)
				maxi = i;
			if(--readyfd <= 0)
				continue;
		}
		
		for(i = 1;i<maxi;i++){
			if((sockfd = clientfd[i].fd) < 0)
				continue;
			
			if(clientfd[i].revents & (POLLRDNORM|POLLERR)){
				if(( n = read(sockfd,buf,MAXLINE)) < 0){
					if(errno == ECONNRESET){
						close(sockfd);
						clientfd[i].fd = -1;
					}else
						perror("read error"),exit(-2);
				}else if(n == 0){
					close(sockfd);
					clientfd[i].fd = -1;
				
				}else
					write(0,buf,n);
				if(--readyfd <= 0)
					break;	
			}
		
		}
	}
}
