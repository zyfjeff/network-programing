#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define ERR_EXIT(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)

int MakeServer(int backlog)
{
	int fd = socket(PF_INET,SOCK_STREAM,0);
		if(fd == -1) ERR_EXIT("Create Socket");
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	//随机地址随机端口
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(0);
	int ret = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
	if(ret == -1) ERR_EXIT("Bind Socket");
	ret = listen(fd,backlog);
	if(ret == -1) ERR_EXIT("turn Socket state to listen");
	return fd;
}


int main()
{
	int fd = MakeServer(100);
	accept(fd,NULL,NULL);
}
