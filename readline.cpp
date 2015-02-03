/*
 *	readline实现，读取一行数据,效率比较低
 *	增加效率需要带有buf
 */

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
ssize_t readline(int fd,void *vptr,size_t maxlen)
{
	ssize_t n,rc;
	char c,*ptr;
	ptr = (char *)vptr;
	for(n = 1;n < maxlen;n++){
		again:
			if((rc = read(fd,&c,1)) == 1){
				*ptr++ = c;
				if(c == '\n')
					break;
			}else if(rc == 0){
				*ptr = 0;//结束
				return(n-1);
			}else {
				if(errno == EINTR)//信号打断 
					goto again;

			return(-1);//错误
			}
			
	}
	*ptr = 0;//加上结束符
	return(n);
}

int main()
{
	char buf[100];
	printf("%d\n",readline(0,buf,100));
}
