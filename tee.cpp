/*
 *	tee系统调用实现tee命令
 */

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>


int main(int argc,char *argv[])
{
	if(argc != 2)
	{
		printf("usage: %s<file>\n",argv[0]);
		return 1;
	}
	//打开文件
	int filefd = open(argv[1],O_CREAT|O_RDWR|O_TRUNC,0666);
	assert(filefd > 0);

	//创建管道,用于输出到标准输出
	int pipefd_stdout[2];
	int ret = pipe(pipefd_stdout);
	assert(ret != -1);

	//再创建一个管道,用于输出到文件
	int pipefd_file[2];
	ret = pipe(pipefd_file);
	assert(ret != -1);
	
	//将标准输出内容 输入管道
	ret = splice(STDIN_FILENO,NULL,pipefd_stdout[1],NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
	assert(ret != -1);

	//将管道pipefd_stdout的输出赋值到文件管道
	ret = tee(pipefd_stdout[0],pipefd_file[1],32768,SPLICE_F_NONBLOCK);
	assert(ret != -1);
	
	//将文件管道中的内容输出到文件
	ret = splice(pipefd_file[0],NULL,filefd,NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
	assert(ret != -1);

	close(filefd);
	close(pipefd_stdout[0]);
	close(pipefd_stdout[1]);
	close(pipefd_file[0]);
	close(pipefd_file[1]);
	return 0;
}	