/*
 *	unix域协议
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
int main(int argc,char *argv[])
{
	int sockfd;
	socklen_t len;
	struct sockaddr_un addr1,addr2;
	if(argc != 2){
		printf("usage: unixbind <pathname>");
	}
	sockfd = socket(AF_UNIX,SOCK_STREAM,0);
	unlink(argv[1]);
	bzero(&addr1,sizeof(addr1));
	addr1.sun_family = AF_UNIX;
	strncpy(addr1.sun_path,argv[1],sizeof(addr1.sun_path)-1);
	bind(sockfd,(struct sockaddr*)&addr1,SUN_LEN(&addr1));
	
	len = sizeof(addr2);
	getsockname(sockfd,(struct sockaddr*)&addr2,&len);
	printf("bound name = %s,return len = %d\n",addr2.sun_path,len);
	exit(0);
}
