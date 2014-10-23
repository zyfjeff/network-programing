/*
 * =====================================================================================
 *
 *       Filename:  selecttest.c
 *
 *    Description:  select函数的简单使用和测试
 *
 *        Version:  1.0
 *        Created:  2014年10月23日 16时38分16秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jeff (jeff), zyfforlinux@163.com
 *        Company:  xupt
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>


#define MAXSIZE 4096

int main()
{
	int readyfd,connfd,listenfd,sockfd;
	int ret,i=0;
	int maxi,maxfd;//最大索引和最大fd
	fd_set cset,allset;
	socklen_t len;
	char buf[MAXSIZE];
	struct sockaddr_in cliaddr,addr;
	int clientfd[FD_SETSIZE]; //存放已连接的client fd
	//socket 创建
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd == -1) perror("create sockte error:"),exit(EXIT_FAILURE);
	
	//bind socket
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	addr.sin_addr.s_addr = htonl(INADDR_ANY); //
	ret = bind(listenfd,(struct sockaddr*)&addr,sizeof(addr));
	if(ret == -1) perror("BIND SOCKET"),exit(EXIT_FAILURE);

	//listen fd
	ret = listen(listenfd,5); 
	if(ret == -1) perror("LISTEN FD"),exit(EXIT_FAILURE);
	FD_SET(listenfd,&allset); //把listenfd加入
	maxfd = listenfd;
	
	//初始化clientdf
	for(i = 0;i < FD_SETSIZE;i++)
		clientfd[i] = -1;
	//poll
	for(;;)
	{
		cset = allset;
		readyfd = select(maxfd+1,&cset,NULL,NULL,NULL); //permanent waiting
		
		//如果listenfd就绪，说明有客户端连接过来
		if(FD_ISSET(listenfd,&cset)){
			len = sizeof(cliaddr);
			//接受客户端
			connfd = accept(listenfd,(struct sockaddr*)&cliaddr,&len);
			//打印客户端ip
			printf("client ip:%s\n",inet_ntoa(cliaddr.sin_addr));
			if(connfd > maxfd) maxfd = connfd; //设置最大fd
			
			for(i = 0;i < FD_SETSIZE;i++)
				if(clientfd[i] < 0){
					clientfd[i] = connfd;
					break;
				}
			if(i == FD_SETSIZE)printf("too many clients\n"),exit(-4);
			FD_SET(connfd,&allset); //加入fd_set
			if(i > maxi)
				maxi = i; //设置client数组的最大索引
			
			if(--readyfd <= 0)
				continue;  //没有client fd就绪
		}
		
		for(i = 0;i<maxi;i++)
		{
			if((sockfd = clientfd[i]) < 0)
				continue;
			
			if(FD_ISSET(sockfd,&cset)) {
				if(( ret = read(sockfd,buf,MAXSIZE)) == 0){
					close(sockfd);
					FD_CLR(sockfd,&allset);
					clientfd[i] = -1;
				}else
					write(0,buf,ret);
					//write(sockfd,buf,ret);
				
				if(--readyfd <= 0)
					break;
				
			}
		}
	}
	
}

