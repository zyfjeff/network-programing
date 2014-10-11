/*
	时间服务器的客户端代码
*/

#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 65535

int main(int argc,char **argv)
{
	int sockfd,n;
	char recvline[MAXLINE+1];
	struct sockaddr_in servaddr;
	
	//参数判断
	if(argc != 2){
		printf("usage: a.out <IPaddress>");
		exit(1);
	}
	//创建socket fd
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
		perror("socket error");

	//清空servaddr
	bzero(&servaddr,sizeof(servaddr));
	
	//设置协议簇
	servaddr.sin_family = AF_INET;
	
	//端口号字节序转换
	servaddr.sin_port = htons(13);

	//inet_addr(argv[1],&servaddr.sin_addr);
	if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr) <= 0)
		perror("inet_pton error");
	
	if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) <0)
		perror("connect error");

	while((n = read(sockfd,recvline,MAXLINE)) >0 ){
		recvline[n] = 0;
		if(fputs(recvline,stdout) == EOF)
			perror("fputs error");
	}
	
	if(n < 0)
		printf("read error\n");

	exit(0);
}
