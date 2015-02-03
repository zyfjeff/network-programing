/*
	getsockname的基本使用
	返回fd的信息,
	getpeername 获取对端连接过来的socket的基本信息
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

#define ERR_EXIT(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)

int sockfd_to_port(int sockfd)
{
	struct sockaddr_in ss;
	socklen_t len;
	len = sizeof(ss);
	if(getsockname(sockfd,(struct sockaddr*)&ss,&len)<0)
		return -1;
	return ss.sin_port;
}

int main()
{
	
	int r;
	struct sockaddr_in addr;
	socklen_t len;
	int fd = socket(AF_INET,SOCK_STREAM,0);
	int value = 1;
        setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(81);
	inet_aton("127.0.0.1",&addr.sin_addr);
	r = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
	//打印本地server的信息
	printf("local server port:%d\n",ntohs(sockfd_to_port(fd)));
	if(r == -1) ERR_EXIT("bind socket");
	r = listen(fd,100);
	if(r == -1) ERR_EXIT("listen socket");
	
	int clfd = accept(fd,NULL,NULL);
	struct sockaddr_in ss;
	char buf[1024];
	len = sizeof(ss);
	//打印连接过来的客户端信息
	getpeername(clfd,(struct sockaddr*)&ss,&len);
	printf("client port:%d\n",ntohs(ss.sin_port));
	printf("client addr:%s\n",inet_ntop(AF_INET,(void*)&ss.sin_addr,buf,INET_ADDRSTRLEN));
	
}
