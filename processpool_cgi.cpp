/*
 =====================================================================================
        Filename:  processpool_cgi.cpp
     	Description:  
        Version:  1.0
        Created:  15/02/15 23:15:29
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet.h>
#include <assert.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h.

#include "processpool.h"

class cgi_conn
{
public:
	cgi_conn(){}
	~cgi_conn(){}
	void init(int epollfd,int sockfd,const sockaddr_in& client_addr)
	{
		m_epollfd = epollfd;
	}

};
