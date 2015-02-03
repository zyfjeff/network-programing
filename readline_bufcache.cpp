#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#define MAXLINE 1024
static int read_cnt;//剩余未读的字节数
static char *read_ptr; //当前读到缓存的位置
static char read_buf[MAXLINE]; //缓存


static ssize_t my_read(int fd,char *ptr)
{
	
	if(read_cnt <= 0) {
		again:
			if((read_cnt = read(fd,read_buf,sizeof(read_buf))) < 0) {
				if(errno == EINTR)
					goto again;
				else
					return(-1);
			}else if(read_cnt == 0)
				return(0);
			else read_ptr = read_buf;
	}
	read_cnt--;
	*ptr = *read_ptr++;
	return(1);
}

//查看缓存区内部的状态
ssize_t readlinebuf(void **vptrptr)
{
	if(read_cnt) //判断是否有缓存
		*vptrptr = read_ptr; //指向缓存区
	return(read_cnt);//判断缓存大小
}
