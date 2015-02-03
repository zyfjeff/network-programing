#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc,char *argv[])
{
	int sockfd;
	char buf[1024];
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	if(argc != 2){
		printf("usage:%s <IPaddress>\n",argv[0]);
		exit(-1);
	}

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
	sockfd = socket(PF_INET,SOCK_DGRAM,0);
	bzero(buf,sizeof(buf));
	connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	while(fgets(buf,sizeof(buf),stdin) != NULL){
		//sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&servaddr,sizeof(servaddr));
		sendto(sockfd,buf,strlen(buf),0,NULL,0);
		int size = recvfrom(sockfd, buf, sizeof(buf), 0,NULL,0);
		printf("size:%d\n",size);
		perror("recvfrom");
		buf[size] = 0;
		fputs(buf,stdout);
	}
}
