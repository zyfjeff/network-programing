/*
 *	回射客户端
 *
 */
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
using namespace std;

#define ERR_EXIT(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)


int MakeConnect(const char *ip,int port)
{
	int ret;
	int sockfd = socket(PF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
		ERR_EXIT("create socket");
	struct sockaddr_in addr;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	if((ret = inet_pton(PF_INET,ip,&(addr.sin_addr))) < 0)
		ERR_EXIT("convert to ip");
	//三次握手建立的过程，建立成功或者建立失败才返回
	if((ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in))) < 0){
		switch(errno){
			//connect连接超时
			case ETIMEDOUT:{
				cout << "connect time out" << endl;
				break;
			}
			//server端没有服务在指定端口监听
			case ECONNREFUSED: {
				cout << "not have program listen" << endl;
				break;
			}
			//目标不可达 本地网络,不存这样的主机
			case EHOSTUNREACH: {
				cout << "host don't attach" << endl;
				break;
			}
			//网络不可达
			case ENETUNREACH: {
				cout << "host don't net" << endl;
				break;
			}
			//其他情况
			default:
				ERR_EXIT("connect to server");
		}
	}
	return sockfd;
}

void handlerpipe(int signum)
{
	cout << "don't write pipe" << endl;
}

int main()
{
	signal(SIGPIPE,handlerpipe);
	int sockfd = MakeConnect("192.168.198.134",8080);
		char line[] = "ds2d2d2d2d21dwq";
                send(sockfd,line,sizeof(line),0);
                bzero(line,sizeof(line));
                int ret = recv(sockfd,line,sizeof(line),0);
		if(ret >0){
                	cout << line << endl;
		}else if(ret == 0){
			cout << "server close" << endl;
		}else if(ret < 0){
			cout << "recv error" << endl;
		}
		
		sleep(20);
                cout << "write to server" << endl;
		ret = send(sockfd,line,sizeof(line),0);
		cout << ret << endl;
		perror("write....");
		bzero(line,sizeof(line));
                sleep(5);
		ret = recv(sockfd,line,sizeof(line),0);
		cout << ret <<line << endl;
		perror("recv....");
		ret = recv(sockfd,line,sizeof(line),0);
		perror("recv....");
		if(ret < 0){
			if(errno == EBADF)
				cout << "connect ret" << endl;
			perror("send");
		
		}
	cout << "end " << endl;
	while(1);
	close(sockfd);
}
