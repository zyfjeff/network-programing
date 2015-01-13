#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>


#define ERR_EXIT(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)

int main()
{
	int sockfd = socket(PF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
		ERR_EXIT("create socket");
	//设置socket选项
	int value = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value));
	int buf = 4096;
	setsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,&buf,sizeof(buf));
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(81);
	if(inet_pton(AF_INET,"192.168.198.130",&(addr.sin_addr)) < 0)
		ERR_EXIT("convert ip");
	bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
	listen(sockfd,10);
	int fd = accept(sockfd,NULL,NULL);	

}

