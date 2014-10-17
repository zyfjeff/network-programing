#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
int sfd;
int *fds;//存放所有客户代理描述符号
int idx=0;//客户在数组中下标
struct sockaddr_in dr;
int r;
main()
{
	//1.建立服务器 socket,匿名映射共享内存区域,存放客户端的fd
	fds=mmap(0,4*100,PROT_READ|PROT_WRITE,
		MAP_ANONYMOUS|MAP_SHARED,0,0);
	//清空分配的内存空间	
	bzero(fds,sizeof(fds));

	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1) printf("1:%m\n"),exit(-1);	
	printf("socket OK!\n");

	//2.绑定地址
	dr.sin_family=AF_INET;
	dr.sin_port=htons(9989);
	dr.sin_addr.s_addr=inet_addr("192.168.64.128");
		
	r=bind(sfd,(struct sockaddr*)&dr,sizeof(dr));
	if(r==-1) printf("2:%m\n"),exit(-1);
	printf("bind ok!\n");
	//3.监听
	r=listen(sfd,10);
	if(r==-1) printf("3:%m\n"),exit(-1);
	printf("listen ok!\n");
	//4.循环接收客户连接
	while(1)
	{
		fds[idx]=accept(sfd,0,0);
		if(fds[idx]==-1) break;
		printf("有客户连接:%d\n",fds[idx]);
		//5.建立一个子进程
		if(fork())
		{
			idx++; //父进程负责循环接收客户端的连接
			continue;
		}
		else
		{		
			//6.子进程任务：接收客户数据并且广播
			char buf[256];
			int i;
			printf("开始接收客户数据:%d\n",fds[idx]);//打印客户端的fd
			while(1)
			{
				//接收客户数据
				r=recv(fds[idx],buf,255,0);
				printf("%d\n",r);
				if(r==0)
				{
					printf("有客户退出\n");
					close(fds[idx]);
					fds[idx]=0;
					break;					
				}
				if(r==-1)
				{
					printf("网络故障\n");
					close(fds[idx]);
					fds[idx]=0;
					break;
				}
				buf[r]=0;
				printf("来自客户的数据:%s\n",buf);
				//广播
				for(i=0;i<100;i++)
				{
					if(fds[i]>0)
					{
						send(fds[i],buf,r,0);
					}	
				}
			}
			exit(0);
		}
	}
	close(sfd);	
}

