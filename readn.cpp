/*
 *	readn实现，避免被信号打断。直到读取到指定大小的数据后才返回
 */

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
ssize_t readn(int fd,void *vptr,size_t n)
{
	size_t nleft;
	size_t nread;
	char *ptr;
	ptr = (char*)vptr;
	nleft = n;
	while(nleft > 0){
		if((nread = read(fd,ptr,nleft)) < 0) {
			if(errno == EINTR)
				nread = 0;  //如果被信号打断,重新读取
			else  		    //read不是因为信号而发生的错误
				return -1;
		}else if(nread == 0)
			break; //EOF 文件读取结束
		nleft -= nread; //减去读过的
		ptr += nread; //buf指针前移
	}
	return (n-nleft); //返回读取到多少字节数据
}

int main()
{
	char buf[100];
	printf("%d\n",readn(0,buf,100));
}
