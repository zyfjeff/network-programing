/*
 =====================================================================================
        Filename:  udpbrosend.c
     	Description:  
        Version:  1.0
        Created:  11/03/15 19:26:06
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */


// 发送端
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


int main()
{
	//设置标准输出 不使用缓存
	setvbuf(stdout, NULL, _IONBF, 0); 
	fflush(stdout); 

	int sock = -1;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{   
		printf("socket error\n");
		return -1;
	}   
	
	const int opt = 1;
	//设置该套接字为广播类型，
	int nb = 0;
	nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	if(nb == -1)
	{
		printf("set socket error...\n");
		return -1;
	}

	struct sockaddr_in addrto;
	bzero(&addrto, sizeof(struct sockaddr_in));
	addrto.sin_family=AF_INET;
	addrto.sin_addr.s_addr=htonl(INADDR_BROADCAST);
	addrto.sin_port=htons(6000);
	int nlen=sizeof(addrto);

		//从广播地址发送消息
		char smsg[] = {"abcdef"};
		int ret=sendto(sock, smsg, strlen(smsg), 0, (struct sockaddr*)&addrto, nlen);
		if(ret<0)
		{
			printf("send error %d\n",ret);
		}
		else
		{		
			printf("ok ");	
		}
        while(1)
        {
           ret =  recvfrom(sock,smsg,strlen(smsg),0,NULL,0);
           smsg[ret] = '0';
           printf("%s\n",smsg);
        }
	return 0;
}

