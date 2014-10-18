/*
	getsockname的基本使用
	返回sockfd的信息,
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>

int sockfd_to_family(int sockfd)
{
	struct sockaddr_storage ss;
	socklen_t len;
	len = sizeof(ss);
	if(getsockname(sockfd,(struct sockaddr*)&ss,&len)<0)
		return -1;
	return ss.ss_family;
}

int main()
{
	
	int r;
	struct sockaddr_in ss;
	struct sockaddr_in addr;
	socklen_t len;
	int fd = socket(AF_INET,SOCK_STREAM,0);
	printf("%d\n",sockfd_to_family(fd));
	printf("%d\n",AF_INET);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(13);
	inet_aton("127.0.0.1",&addr.sin_addr);
	//inet_pton(AF_INET,"192.168.64.128",&addr.sin_addr);
	r = connect(fd,(struct sockaddr*)&addr,sizeof(addr));
	if(r != -1){
		//打印sockket信息
	    len = sizeof(ss);
	    if(getsockname(fd,(struct sockaddr*)&ss,&len)<0)
                return -1;
	printf("%s\n",inet_ntoa(ss.sin_addr));
	    
	}else perror("connect");
	
	
}
