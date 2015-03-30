/*
 =====================================================================================
        Filename:  string.c
     	Description:  linux C下的字符串处理函数
        Version:  1.0
        Created:  16/02/15 09:32:27
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <stdio.h>
#include <strings.h>
#include <string.h>
#define _GNU_SOURCE

int main()
{
//BSD贡献
//char *index(const char *s, int c);
//char *rindex(const char *s, int c);
//index 返回字符串首次出现字符x的位置
//rindex则是逆序的
	char name[] = " zhang yi fei";
	char *pos = index(name,'y');
	*pos = 'W';
	printf("%s\n",name);
	//输出结果: zhang Wi fei

//void *memchr(const void *s, int c, size_t n);
// void *memrchr(const void *s, int c, size_t n);
//memchr 类似于index，返回内存中前N个字节中，出现字符x的位置
//memrchr 逆序
	pos = memchr(name,'W',sizeof(name));
	if(pos != NULL)
		*pos = 'Z';
	printf("%s\n",name);
	//输出结果: zhang Zi fei

//strlen strcmp strncmp strcpy strncpy这些都略过
//strcasecmp strncasecmp(不区分大小的比较)
//strcat strncat strchr strnchr strcpy strncpy 字符串连接，获取指定字符的位置(glibc版本,index是BSD版本) 字符串拷贝

//strcoll 使用当前语系来比较字符串，和strcmp机制不同(strcmp只是单纯比较字符的大小)


//size_t strspn(const char *s, const char *accept);
//size_t strcspn(const char *s, const char *reject);
// strspn 搜索在accept中出现的字符，出现在s的开始位置处的字节数
// strcspn 则是相反,不出现在rejecet中的字符 出现在s的开始位置处的字节数
	printf("%d\n",strspn(name," "));
	//返回1，因为 name=" zhang Zi fei" 开头只有一个空白字符

//char *strdup(const char *s);
// strdup拷贝字符串
	char *newname = strdup(name);
	printf("%s\n",name);
	printf("%s\n",newname);
	//结果一样
	newname[1] = 'Q'; //深拷贝，不影响name
	printf("%s\n",name);
//char *strpbrk(const char *s, const char *accept);
//strpbrk 返回accept中出现的字符在s中第一次出现的位置
	char *a = strpbrk(name,"z1");
	*a = '%';
	printf("%s\n",name);
	//输出结果:  %hang Zi fei
//char *strsep(char **stringp, const char *delim);
//

}

