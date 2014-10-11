/*
	时间服务器
*/
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define MAXLINE 100

int main()
{
	int listenfd,connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;
	socklen_t len;
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(13);

	//端口和ip地址的绑定
	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	//监听
	listen(listenfd,2);
	len = sizeof(servaddr);
	for( ; ;){
		//循环的接受
		connfd = accept(listenfd,(struct sockaddr *)&servaddr,&len);
		ticks = time(NULL);//get current time
		//打印ip地址
		printf("%s\n",inet_ntoa(servaddr.sin_addr));
		snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
		write(connfd,buff,strlen(buff));
		close(connfd);
	}	
}
