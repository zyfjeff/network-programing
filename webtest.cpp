/*
 =====================================================================================
        Filename:  webtest.cpp
     	Description:  压力测试工具
        Version:  1.0
        Created:  14/03/15 13:22:39
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

static const char* request = "GET http://localhost/index.html HTTP/1.1\r\nConnection: keep-alived\r\n\r\nxxxxxxxxxxx";

int setnonblocking(int fd);
{
	int old_option = fcntl(fd,F_GETFL);
	int new_option = old_option|O_NONBLOCK;
	
}
