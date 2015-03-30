/*
 =====================================================================================
        Filename:  var_array.c
     	Description:  可变数组
        Version:  1.0
        Created:  14/02/15 13:10:21
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <stdarg.h>
#include <stdio.h>

int test(int count,...)
{
	int total = 0;
	int i = 0;
	va_list ap;
	va_start(ap,count);
	for(i = 0;i < count;i++)
	{
		printf("%d\n",va_arg(ap,int));
	}
}


int main()
{
	test(5,1,2,3,4,5);
}
