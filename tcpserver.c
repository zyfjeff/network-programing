/*
 *
 *	TCP服务器
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
	int serverfd;
	int clientfd;
	struct sockaddr_in saddr;
	struct sockaddr_in caddr;
	int r;
	char buf[4096];
	socklen_t len;
	//socket
	if((serverfd = socket(AF_INET,SOCK_STREAM,0)) == -1)perror("create socket:"),exit(-1); // error from but i don't kown reason
	/*
	serverfd = socket(AF_INET,SOCK_STREAM,0);
	if(serverfd == -1)
		perror("create socket: "),exit(-1);
	*/
	printf("建立服务器成功\n");
	//bind
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8888);
	inet_aton("202.117.132.178",&saddr.sin_addr);
	len = sizeof(saddr);

	if((r = bind(serverfd,(struct sockaddr*)&saddr,len)) == -1)perror("bind error\n"),exit(-2);
	printf("服务器绑定成功\n");
	//listen
	if((r = listen(serverfd,10)) == -1)perror("listen error\n"),exit(-3);
	len = sizeof(caddr);
	while(1){
		//accept
		clientfd = accept(serverfd,(struct sockaddr*)&caddr,&len);
		if(clientfd != -1){
		//print client info
			printf("%d\t%s\n",clientfd,inet_ntoa(caddr.sin_addr));
			printf("端口号:%d\n",ntohs(caddr.sin_port));

		//read client data
			bzero(buf,sizeof(buf));
			r = read(clientfd,buf,sizeof(buf)-1);
				if(r > 0){
					buf[r] = 0;
					printf("%s\n",buf);
				}
				if(r == 0){
					printf("read data EOF:\n");
					break;
				}
				if(r == -1){
					printf("network error\n");
					break;
				}
					
	
		//write client data
			bzero(buf,sizeof(buf));
			snprintf(buf,sizeof(buf),"%s","Hello World");
			write(clientfd,"<h1>Hello World</h1>",strlen("<h1>Hello World</h1>"));
	
		//close
			close(clientfd);
		}
			
		else{
			perror("accept error");
			exit(-4);
		}
	}
	

}
