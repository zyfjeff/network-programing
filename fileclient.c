#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main()
{
	int fd;
	int ret,filefd,len;
	int size;
	char buf[128];
	struct sockaddr_in addr;
	char filename[]="/tmp/text.c";
	//socket 创建
	if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1)perror("create error"),exit(-1);
	printf("socket create success\n");
	
	//connect server端
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	inet_aton("202.117.132.178",&addr.sin_addr);
	
	//打开文件
	if((ret = access(filename,F_OK) == 0))
	{
		//文件存在
		filefd = open(filename,O_RDWR);
	}else{
		//文件不存在
		printf("file not exists\n");
		exit(-3);
	}
	if((ret = connect(fd,(struct sockaddr*)&addr,sizeof(addr))) == -1)perror("connect error"),exit(-2);
	printf("connect success\n");
	
	//发送文件名的长度
	len = strlen(filename);
	ret = send(fd,&len,sizeof(len),0);
	if(ret == -1)printf("send error\n"),exit(-4);
	//发送文件名
	ret = send(fd,filename,len,0);
	if(ret == -1)printf("send filename error\n"),exit(-5);
	
	//循环发送数据
	while(1){
		//先读取数据
		size = read(filefd,buf,sizeof(buf));
		if(size <= 0)break;
		//发送数据长度
		ret = send(fd,&size,sizeof(size),0);
		if(ret < 0)break;
		//发送数据
		ret  = send(fd,buf,size,0);
		if(ret <0)break;

	}
	close(filefd);
	close(fd);
	return 0;
}
