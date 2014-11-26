/*
 * =====================================================================================
 *
 *       Filename:  select.c
 *
 *    Description:  select练习
 *
 *        Version:  1.0
 *        Created:  2014年11月24日 21时43分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dr. Fritz Mehner (mn), mehner@fh-swf.de
 *        Company:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	//保存最大描述符
	int maxfd = 0;
	int fd;
	int clientfd[FD_SETSIZE]; //用来保存客户端的fd
	fd_set readfds;
	fd_set allfds; //是一个readfds的副本。因为每次内核都会修改readfds,需要重新置位
	struct sockaddr_in addr;
	struct sockaddr_in client_addr;
	char buf[1024];
	char ipbuf[1024];
	int i = 0;
	int addrlen;
	//描述符初始化
	FD_ZERO(&readfds);
	FD_ZERO(&allfds);
	
	bzero(clientfd,sizeof(clientfd));
	bzero(buf,sizeof(buf));
	bzero(ipbuf,sizeof(ipbuf));
	//创建listenfd
	int listenfd = socket(PF_INET,SOCK_STREAM,0);
	
	if(-1 == listenfd){ perror("create socket error");exit(-1);}

	if(listenfd > maxfd) maxfd = listenfd; //获取描述符编号的最大值

	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	addr.sin_addr.s_addr = htonl(INADDR_ANY); //监听在任意地址
	
	//绑定套接字到指定端口
	int ret = bind(listenfd,(struct sockaddr *)&addr,sizeof(struct sockaddr));
	if(-1 == ret){ perror("bind socket error");exit(-1);}
	
	ret  = listen(listenfd,10); //开始监听
	//将监听描述符加入集合
	FD_SET(listenfd,&allfds);
	int client_num;
	addrlen = sizeof(struct sockaddr);
	//select开始监听
	while(1){
		readfds = allfds; //每次循环重新给readfds置位
		client_num = select(maxfd+1,&readfds,NULL,NULL,NULL);
		if(client_num < 0){printf("select error \n");break;}
		if(FD_ISSET(listenfd,&readfds)){ //如果是监听描述符
			//获取客户端连接过来产生的fd
			fd = accept(listenfd,(struct sockaddr *)&client_addr,&addrlen);
			if(fd < 0){ printf("accept socket error");continue;}
			FD_SET(fd,&allfds);
			if(fd > maxfd)maxfd = fd;
			//打印客户端地址
			printf("ip:%s \t port:%d",inet_ntop(AF_INET,&client_addr.sin_addr,ipbuf,INET_ADDRSTRLEN),ntohs(client_addr.sin_port));
			for(i = 0;i < FD_SETSIZE;i++)
				if(clientfd[i] == 0){
					clientfd[i] = fd; //保存客户端的这些fd
					break;
				}
			if(--client_num <= 0) //处理结束了，进入下次循环
				continue;
		}
		//如果是客户端有数据可读了，则读取数据
		for(i = 0 ; i< maxfd;i++){
			if(0 == (fd = clientfd[i])) //寻找客户端的fd
				continue;
			if(FD_ISSET(fd,&readfds)){  //判断这个fd是否可读
				if((ret = read(fd,buf,sizeof(buf))) == 0) {
					close(fd); //连接被客户端关闭了
					FD_CLR(fd,&allfds); //从描述符集合中清除
					clientfd[i] = 0;
				}else
					printf("\t write data to fd\n");
				if(--client_num <= 0) //说明处理结束了，直接退出for循环
					break;
			}
		}
	}
	close(listenfd);
	
}
