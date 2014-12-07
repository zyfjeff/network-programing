/*
 *	splice系统调用的简单实现
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
        char buf[BUFSIZ];
        int n=0;
	int p[2];
        const char *filename1 = "in.txt";
        const char *filename2 = "out.txt";
	int ret = pipe(p);
	if( ret == -1){
		perror("create pipe failue");
		exit(-1);
	}
        int fd = open(filename1,O_RDWR);
        int fd2 = open(filename2,O_RDWR);
        struct stat file_buf;
        fstat(fd,&file_buf);
	
       ret = splice(fd,NULL,p[1],NULL,file_buf.st_size,SPLICE_F_MORE|SPLICE_F_MOVE);
	if( ret == -1){
		perror("use splice failue");
		exit(-1);
	}
	
        ret = splice(p[0],NULL,fd2,NULL,file_buf.st_size,SPLICE_F_MORE|SPLICE_F_MOVE);
	if( ret == -1){
		perror("use splice failue");
		exit(-1);
	}
}
