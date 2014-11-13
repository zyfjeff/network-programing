/*
 *
 *	getaddinfo应用一
 *
 */

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct addrinfo *host_serv(const char *host,const char *serv,int family,int socktype)
{
	int n;
	struct addrinfo hints,*res;
	bzero(&hints,sizeof(struct addrinfo))	;
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = family;
	hints.ai_socktype = socktype;
	
	if((n = getaddrinfo(host,serv,&hints,&res)) != 0)
		return NULL;
	return res;
}

/*
 * 通过host和serv返回一个可以 由tcp来连接的套接字
 */
int tcp_connect(const char *host,const char *serv)
{
	int sockfd,n;
	struct addrinfo hints,*res,*ressave;
	bzero(&hints,sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if((n = getaddrinfo(host,serv,&hints,&res)) != 0)
		perror("tcp_connect error:"),exit(-1);

	ressave = res;

	do {
		sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if(sockfd < 0)
			continue;

		if(connect(sockfd,res->ai_addr,res->ai_addrlen) == 0)
			break;
		close(sockfd);
	}while((res = res->ai_next) != NULL);

	if(res == NULL)
		perror("getaddrinfo error"),exit(-2);

	freeaddrinfo(ressave);
	return sockfd;
}

int main()
{
	struct addrinfo *res;
	res = host_serv("localhost","tcp",AF_INET,SOCK_STREAM);
	printf("%s\n",res->ai_canonname);
}
