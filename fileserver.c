/*
 *文件接受服务器
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int fd;
	int r,cfd;
	int filefd;
	socklen_t len;
	int datalen;
	char buf[128];
	struct sockaddr_in addr;
	struct sockaddr_in caddr;
	char filename[100];
	char *name;
	//1.建立服务器socket
	if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1)perror("1:socket creat error"),exit(-1);
	printf("建立服务器成功\n");

	//2.绑定一个ip地址
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	//两种方式
	inet_aton("202.117.132.178",&addr.sin_addr); //方式一 推荐
	//addr.sin_addr.s_addr = inet_addr("202.117.132.178"); //方式二
	if((r = bind(fd,(struct sockaddr*)&addr,sizeof(addr))) == -1)perror("bind addr error"),exit(-2);
	printf("绑定服务器成功\n");
		
	//3.在套接字上开始监听
	if((r = listen(fd,10)) == -1)perror("listen error"),exit(-3);
	printf("监听成功\n");

	//4.接收连接
	len = sizeof(caddr);
	if((cfd = accept(fd,(struct sockaddr*)&caddr,&len)) == -1)
	printf("开始接收文件\n");

	//5.接受文件名
	r = recv(cfd,&datalen,sizeof(datalen),MSG_WAITALL);
	r = recv(cfd,filename,datalen,MSG_WAITALL);
	filename[datalen] = 0; //加入终止符
	printf("传递的文件名是%s\n",filename);
	name = filename+5;
	//6.创建文件
	filefd = open(name,O_EXCL|O_CREAT|O_RDWR,0666);
	if(filefd == -1) //说明文件存在
	{
		filefd = open(filename,O_TRUNC|O_RDWR); //覆盖读写
	}
	//7.循环接收文件数据
	while(1)
	{
		r = recv(cfd,&datalen,sizeof(datalen),MSG_WAITALL);
		if(r == 0)break;
		r = recv(cfd,buf,datalen,MSG_WAITALL);
		write(filefd,buf,datalen);
	}
	close(filefd);
	close(cfd);
	close(fd);

}
