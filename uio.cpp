/*
 *	高级IO
 *
 */

//结合一种场景来描述
/*
 *	在web server中通常需要响应一个http head和 http body,通常http head是通用的
 *	所以http head单独使用一个buf存放。那么可以使用writev来实现对http head和body一次性发送给用户
 *	这只是一个比较常用的一种场景。
 */
#include <sys/uio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
//将两块不是连续的内存区域的数据写入文件

int main()
{
	struct iovec data[2];
	char buf1[] = "buf1";
	char buf2[] = "buf2";
	int fd = open("1.txt",O_CREAT|O_EXCL|O_RDWR,0777);
	if(fd < 0)
	{
		//文件已经存在
		fd = open("1.txt",O_RDWR);
	}
	data[0].iov_base = buf1;
	data[0].iov_len = strlen(buf1);
	data[1].iov_base = buf2;
	data[1].iov_len = strlen(buf2);
	writev(fd,data,2);
	close(fd);
}
