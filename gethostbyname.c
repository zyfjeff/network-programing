#include <sys/stat.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
static const char *skip(const char *in) {while (in && *in && (unsigned char)*in<=32) in++; return in;}

int main(int argc,char **argv)
{
	char *ptr,**pptr;
	//地址的最大长度
	char str[INET_ADDRSTRLEN];

	//接受gethostbyname的结果
	struct hostent *hptr;

	//轮询对输入的每一个域名进行解析
	while(--argc > 0) {
		ptr = *++argv;
		//进行域名解析，如果出错就打印出错信息
		if((hptr = gethostbyname(ptr)) == NULL) {
			printf("gethostbyname(%s)\n",hstrerror(h_errno));
			continue;
		}
		//打印正式名
		printf("offical hostname:%s\n",skip(hptr->h_name));
		//遍历打印所有的地址别名
		for(pptr = hptr->h_aliases;*pptr != NULL;pptr++)
			printf("alias: %s\n",skip(*pptr));

		//判断地址类型
		switch(hptr->h_addrtype) {
			//打印ipv4地址
			case AF_INET:
				pptr = hptr->h_addr_list;
				//遍历打印
				for(; *pptr != NULL;pptr++)
					//输出地址的字符串形式
					printf("address: %s\n",inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str)));
				break;
			default:
				printf("unknown address type\n");
				break;
		
		}
		
	}
	exit(0);
}
