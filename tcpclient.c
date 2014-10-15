/*
 *	TCP客户端
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>


int main()
{
	int fd;
	struct sockaddr_in saddr;
	int r,i;
	socklen_t len;
	//socket
	if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1)perror("create socket:"),exit(-1); // error from but i don't kown reason
	printf("建立socket描述符\n");
	
	//connect
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8888);
	inet_aton("202.117.132.178",&saddr.sin_addr);
	len = sizeof(saddr);

	if((r = connect(fd,(struct sockaddr*)&saddr,len)) == -1)perror("connect error\n"),exit(-2);
	printf("服务器连接成功\n");
	
	for(i=0;i<20;i++)
	{
	//发送数据
		send(fd,&i,4,0);
	}

}
