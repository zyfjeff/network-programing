#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <neyinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>


int main
{
	int fsd; //服务器描述符号
	int fdall[100]; //客户描述符号
	int count; //客户个数
	int r; //返回值
	struct sockaddr_in dr; //IP地址与端口
	fd_set fds; //被select监控的描述符集合
	int maxfd;//最大的文件描述符合
	int i,j;//循环变量
	char buf[1024];
	
	//1.建立socket
	sfd = socket(AF_INET,SOCK_STREAM,6);//TCP 0和6
	if(sfd == -1)printf("1:%m\n"),exit(-1);

	//2.绑定地址与端口
	dr.sin_family = AF_INET;
	dr.sin_port = htons(8866);
	inet_aton("192.168.64.28",&dr.sin_addr);
	r = bind(sfd,(struct sockaddr*)&dr,sizeof(dr));
	if(r == -1)printf("2:%m\n"),close(sfd),exit(-2);
	//3.监听
	r = listen(sfd,5);
	if(r == -1)printf("3:%m\n"),close(sfd),exit(-3);
	count = 0;
	maxfd = 0;
	FD_ZERO(&fds);
	for(i=0;i<100;i++)
	{
		fdall[i] = -1;
	}
	while(1)
	{
		//4.构造监听的描述符集合
		FD_ZERO(&fds);
		maxfd = 0;
		FD_SET(sfd,&fds);
		maxfd = maxfd >= sfd?maxfd:sfd;
		for(i=0;i<count;i++)
		{
			if(fdall[i] != -1)
			{
				FD_SET(fdall[i],&fds);
				maxfd = maxfd >= fdall[i]?maxfd:fdall[i];
			}
		}
		//5.使用select循环控制符号集
		r = select(maxfd+1,&fds,0,0,0);
		//6.分两种情况处理
		if(FD_ISSET(sfd,&fds))
		{
			fdall[count] = accept(sfd,0,0);
			printf("有客户连接\n");
			count++;
		}
		//7.有客户连接:服务器描述符合
		//8.有客户发送数据:客户描述符号
		for(i = 0;i< count;i++)
		{
			if(fdall[i] != -1 && FD_ISSET(fdall[i],&fds))
			{
				//读取数据
				r = recv(fdall[i],buf,sizeof(buf)-1,0);
				if(r == 0){
					printf("有客户退出\n");
					close(fdall[i]);
					fdall[i] = -1;
				}
				if(r == -1){
					printf("网络故障\n");
					close(fdall[i]);
					fdall[i] = -1;
				}
				if(r >0){
					buf[r] = 0;
					//广播数据
					   for(j=0;j<count;j++)
					{
						if(fdall[j] != -1)
						{
						    send(fdall[j],buf,r,0);
						}
					}					
				}
			}
		}
	
	}	
}

