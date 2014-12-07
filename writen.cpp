/*
 *	writen实现，避免被信号打断。直到读取到指定大小的数据后才返回
 */

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
ssize_t writen(int fd,void *vptr,size_t n)
{
	size_t nleft;
	size_t nwritten;
	const char *ptr;
	ptr = (const char*)vptr;
	nleft = n;
	while(nleft > 0){
		if((nwritten = write(fd,ptr,nleft)) <= 0) {
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;  //如果被信号打断,重新读取
			else  		    
				return -1;	//writen不是因为信号而发生的错误
		}
		nleft -= nwritten; //减去已经写入的字节
		ptr += nwritten; //指针前移
	}
	return (n-nleft);	//返回实际写入多少字节
}

int main()
{
	char buf[] = "zhangyifei";
	printf("%d\n",writen(1,buf,strlen(buf)));
}
