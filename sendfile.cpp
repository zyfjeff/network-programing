/*
 *	sendfile系统调用
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
	char buf[BUFSIZ];
	int n=0;
	const char *filename1 = "in.txt";
	const char *filename2 = "out.txt";
	int fd = open(filename1,O_RDWR);
	int fd2 = open(filename2,O_RDWR);
	struct stat file_buf;
	fstat(fd,&file_buf);
	sendfile(fd2,fd,NULL,file_buf.st_size);
//	while((n = read(fd,buf,BUFSIZ)) > 0)
//	{
//		write(fd2,buf,BUFSIZ);
//	}
//	if(n == 0)
//		printf("cp complete\n");
//	else
//		printf("cp failue\n");

}
