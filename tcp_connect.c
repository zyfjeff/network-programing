#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>




int tcp_connect(const char *host,const char *serv)
{
	int sockfd;
	struct addrinfo hints,*res,*ressave;
	bzero(&hints,sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if((n = getaddrinfo(host,serv,&hints,&res)) != 0)
		printf("%s\n",gai_strerror(n));

	do {
		sockfd = socket(res->ai_family,res->ai_soktype,res->ai_protocol);
		if(sockfd < 0)
			continue;
		if(connect(sockfd,res->ai_addr,res->ai_addrlen) == 0)
			break;
		close(sockfd);
	}while((res = res->ai_next) != NULL);

	if(res == NULL)
		
}

int main()
{
	
}
