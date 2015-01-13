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
	if((ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in))) < 0){
		switch(errno){
			case ETIMEDOUT:{
				cout << "connect time out" << endl;
				break;
			}
			case ECONNREFUSED: {
				cout << "not have program listen" << endl;
				break;
			}
			case EHOSTUNREACH: {
				cout << "host don't attach" << endl;
				break;
			}
			case ENETUNREACH: {
				cout << "host don't net" << endl;
				break;
			}
			default:
				ERR_EXIT("connect to server");
		}
	}
	return sockfd;
}


int main()
{
	int sockfd = MakeConnect("192.168.198.132",81);
	while(1);
/*
	while(1)
	{
		char line[1024];
                bzero(line,sizeof(line));
                fgets(line,sizeof(line),stdin);
                send(sockfd,line,sizeof(line),0);
                bzero(line,sizeof(line));
                int ret = recv(sockfd,line,sizeof(line),0);
		if(ret >0){
                	cout << line << endl;
		}else if(ret == 0){
			cout << "server close" << endl;
			send(sockfd,"dwdwqdwq",8,0);
			break;
		}else if(ret < 0){
			cout << "recv error" << endl;
			break;
		}
	}
*/
	close(sockfd);
}
