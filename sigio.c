/*
 *	信号驱动式IO
 *
 */

#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define ERR_EXIT(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)

void deal_with(int signum)
{
	
	printf("hello world\n");
}

int MakeServer(int backlog)
{
        int ret;
        int sockfd = socket(PF_INET,SOCK_STREAM,0);
        if(sockfd < 0)
                ERR_EXIT("create socket");
        struct sockaddr_in addr;
        addr.sin_port = htons(80);
        addr.sin_family = AF_INET;
        if((ret = inet_pton(PF_INET,"0.0.0.0",&(addr.sin_addr))) < 0)
                ERR_EXIT("convert to ip");
        int value = 1;
        if((ret = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value))) < 0)
                ERR_EXIT("set socket");
        if((ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))) < 0)
                ERR_EXIT("bind socket");
        if((ret = listen(sockfd,backlog)) < 0)
                ERR_EXIT("listen socket");
        return sockfd;
}


void setnonblock(int sockfd)
{
	int oldstatus;
	signal(SIGIO,deal_with);
	fcntl(sockfd,F_SETOWN,getpid());
	fcntl(sockfd,F_GETFL,&oldstatus);
	fcntl(sockfd,F_SETFL,oldstatus|O_ASYNC);
}

int main()
{
	int sockfd = MakeServer(100);
	setnonblock(sockfd);
//	accept(sockfd,NULL,NULL);
	printf("start listen.........\n");
	while(1);
}
