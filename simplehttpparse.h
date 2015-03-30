/*
 * =====================================================================================
 *
 *       Filename:  simplehttpparse.h
 *
 *    Description:  简易的http协议的解析器
 *
 *        Version:  1.0
 *        Created:  17/02/15 21:43:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jeff (), zyfforlinux@163.com
 *   Organization:  Linux
 *
 * =====================================================================================
 */
#ifndef _SIGMPLEHTTPPARSE_H_
#define _SIGMPLEHTTPPARSE_H_

class httpparse
{
public:
	enum CHECK_STATE {CHECK_STATE_REQUESTLINE=0,CHECK_SATTE_HEADER,CHECK_STATE_CONTENT};
	enum LINE_STATUS {LINE_OK = 0,LINE_BAD,LINE_OPEN};
	httpparse(char* _buf,int len):buf(_buf),index(len-1),checkindex(0){};
	LINE_STATUS parse_line();
	parse_requestline();
	parse_head();
private:
	char *buf;
	int index;
	int checkindex;
	LINE_STATUS line_state;
	CHECK_STATE check_state;
	
	
};

#endif //_SIGMPLEHTTPPARSE_H_
