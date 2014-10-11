/*
	域名解析,获取指定域名对应的别名和地址。
*/
#include <stdio.h>
#include <netdb.h>

int main()
{
	struct hostent *ent = NULL;
	ent = gethostbyname("www.baidu.com");
	printf("%s\n",ent->h_aliases[0]);
	printf("%hhu.%hhu.%hhu.%hhu\n",ent->h_addr_list[1][0],ent->h_addr_list[1][1],ent->h_addr_list[1][2],ent->h_addr_list[1][3]);
}
