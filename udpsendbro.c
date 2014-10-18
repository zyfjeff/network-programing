/*
	udp广播发送报文,发送端
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int fd;
	int opt = 1;
	int r;
	struct sockaddr_in dr;
	fd = socket(AF_INET,SOCK_DGRAM,0);
	if(fd == -1)printf("1:%m\n"),exit(-1);
	
	r = setsockopt(fd,SOL_SOCKET,SO_BROADCAST,&opt,sizeof(opt));
	if(r == -1)printf("2:%m\n"),exit(-1);
	dr.sin_family = AF_INET;
	dr.sin_port = htons(9999);
	dr.sin_addr.s_addr = inet_addr("192.168.64.255");
	
	r = sendto(fd,"hello",5,0,(struct sockaddr*)&dr,sizeof(dr));
	if(r == -1)printf("3:%m\n");
	close(fd);
}
