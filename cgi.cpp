/*
 *	实现一个简易的CGI服务器
 *	CGI 编程(把标准输出重定向到一个网络连接)
 */

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

using namespace std;

int main(int argc,char *argv[])
{
	if(argc <= 2)
	{
		printf("usage: %s ip_address port_number\n",basename(argv[0]));
		return 1;
	}
	const char *ip = argv[1];
	int port = atoi(argv[2]);
	struct sockaddr_in address;
	bzero(&address,sizeof(struct sockaddr_in));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET,ip,&(address.sin_addr));
	int sock = socket(PF_INET,SOCK_STREAM,0);
	assert(sock >=0 );
	
	int ret = bind(sock,(struct sockaddr*)&address,sizeof(struct sockaddr_in));
	assert(ret != -1);

	ret = listen(sock,5);
	assert(ret != -1);

	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client);
	
	int connfd = accept(sock,(struct sockaddr*)&client,&client_addrlength);
	if(connfd < 0)
	{
		cout << "errno is: %d\n" << errno << endl;
	}
	else
	{
		close(STDOUT_FILENO);
		dup(connfd);
		cout << "abcd" << endl;
		close(connfd);
	}
	close(sock);
	return 0;
}


