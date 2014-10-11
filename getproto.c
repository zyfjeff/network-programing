/*
	获取指定协议的协议号
*/
#include<netdb.h>
#include<stdio.h>


int main()
{
	struct protoent *ent;
	ent = getprotobyname("tcp");
	printf("%u",ent->p_proto);
}

