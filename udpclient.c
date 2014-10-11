/*
	udp的接收程序
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int fd;
	struct sockaddr_in ad;
	char buf[100];
	struct sockaddr_in ad_snd;
	int r;
	socklen_t len;
	
	//建立socket
	fd = socket(AF_INET,SOCK_DGRAM,17);
	if(fd == -1)printf("socket:%m\n"),exit(-1);
	printf("建立socket成功\n");
	//填充sockaddr_in 结构体
	ad.sin_family = AF_INET;
	ad.sin_port = htons(11111);
	//地址转换和填充
	inet_aton("192.168.1.126",&ad.sin_addr);
	r = bind(fd,(struct sockaddr*)&ad,sizeof(ad));
	if(r == -1)printf("bind err:%m\n"),exit(-1);
	printf("绑定成功\n");
	//循环接受
	while(1)
	{
		len = sizeof(ad_snd);
		r = recvfrom(fd,buf,sizeof(buf)-1,0,(struct sockaddr*)&ad_snd,&len);
		if(r>0){
			buf[r] = 0;
			printf("发送者IP:%s\n",inet_ntoa(ad_snd.sin_addr));
			printf("数据:%s\n",buf);
		}
	
		if(r == 0)
		{
			printf("发送者关闭\n");
			close(fd);
			break;
		}

		if(r == -1)
		{
			printf("网络故障\n");
			close(fd);
			break;
		}
		
	}
		close(fd);
}
