/*
 *	getservbyname的使用
 */
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc,char *argv[])
{
	assert(argc == 2);
	char *host = argv[1];
	struct hostent* hostinfo = gethostbyname(host);
	assert(hostinfo);
	//获取服务信息
	struct servent* servinfo = getservbyname("daytime","tcp");
	assert(servinfo);
	//获取服务的端口号
	printf("daytime port is %d\n",ntohs(servinfo->s_port));
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = servinfo->s_port;
	//获取host对应的地址
	address.sin_addr =*(struct in_addr*)*hostinfo->h_addr_list;
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	//连接过去
	int result = connect(sockfd,(struct sockaddr*)&address,sizeof(address));
	assert(result != -1);
	char buffer[128];
	//读取数据
	result = read(sockfd,buffer,sizeof(buffer));
	assert(result > 0);
	buffer[result] = '\0';
	//显示数据
	printf("the day tiem is:%s",buffer);
	close(sockfd);
	return 0;
}
