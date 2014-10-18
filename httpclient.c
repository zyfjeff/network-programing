/*
	http的client端，模拟浏览器访问网页获得其结果
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
	struct sockaddr_in addr;
	int fd;
	char buf[150];
	char data[5000];
	int ret;
	int n;
	
	//socket
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd == -1)perror("Creat Socket:"),exit(-1);
	printf("socket connnect success\n");
	
	//connect
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	inet_aton("180.97.33.108",&addr.sin_addr);
	ret = connect(fd,(struct sockaddr*)&addr,sizeof(addr));
	if(ret == -1)perror("connect:"),exit(-2);
	printf("connect success");
	
	//构造http协议头部
	sprintf(buf,
		"GET / HTTP/1.1\r\n"
		"Host: www.baidu.com\r\n"
		"User-Agent: Tarena5.0\r\n"
		"Accept: text/html,image/png\r\n"
		"Accept-Language: zh-cn\r\n"
		"Accept-Charset: gb2312,utf-8\r\n"
		"Keep-Alive: 300\r\n"
		"Connection: keep-alive\r\n"
		"\r\n"); //最后一个

//	snprintf(buf,sizeof(buf),"GET / http/1.1\r\nHost:www.baidu.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:32.0) Gecko/20100101 Firefox/32\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: zh-cn\r\nAccept-Encoding: gzip, deflate\r\n");

	//发送数据
	ret = send(fd,buf,strlen(buf),0);
	if(ret == -1)perror("send data error"),exit(-3);
	n = recv(fd,buf,sizeof(data)-1,MSG_WAITALL);
	data[n] = 0;
	printf("%s\n",data);
	close(fd);
	
	
}
