/*
	获取系统的信息
*/
#include<stdio.h>
#include <sys/utsname.h>
int main()
{
	struct utsname name;
	uname(&name);
	printf("%s\n",name.sysname);
	printf("%s\n",name.nodename);
	printf("%s\n",name.release);
	printf("%s\n",name.version);
	printf("%s\n",name.machine);
//	printf("%s\n",name.domainname);
	
}
