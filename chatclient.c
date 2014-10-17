#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <curses.h>
#include <signal.h>
WINDOW*winfo,*wmsg; //指定两个窗口

int fd; //socket fd
int r; //保存返回值
struct sockaddr_in   dr; //保存ipv4地址
int isover=1; //标志

int initSocket(); //初始化socket,返回生成的fd,出错返回-1
void initUI();  //初始化UI
void destroy(); //资源回收
void handle(int s); //信号处理函数，对子进程进行回收

//信号处理函数,wait子进程，
void handle(int s)
{
	int status;
	wait(&status);
	destroy();
	exit(-1);	
}

main()
{	
	initUI();//初始化curses
	r=initSocket();	//初始化socket
	if(r==-1) exit(-1);
	signal(SIGCHLD,handle); //子进程回收
	if(fork())
	{
	//父进程,负责发送数据
		//存放数据的buf
		char buf[256];
		while(1)
		{
			mvwgetstr(wmsg,1,1,buf);
			//buf[r]=0;
			send(fd,buf,strlen(buf),0);			
			//wclear(wmsg);
			//box(wmsg,0,0);
			refresh();
			wrefresh(wmsg);
			wrefresh(winfo);
		}
	}
	else
	{
		
		//负责接收数据和显示数据
		//数据存放的buf
		char buf[256];
		int line=1;
		while(1)
		{			
			r=recv(fd,buf,255,0);
			if(r==-1) break;
			if(r==0) break;
			buf[r]=0;
			//绘制数据
			mvwaddstr(winfo,line,1,buf);
			line++;			
			if(line>=(LINES-3))
			{
				//清屏
				wclear(winfo);
				//重新设置输出的行号和边界
				line=1;
				box(winfo,0,0);				
			}
			//指定光标的位置
			wmove(wmsg,1,1);
			//
			touchwin(wmsg);
			
			refresh();
			wrefresh(winfo);			
			wrefresh(wmsg);
		}
		exit(-1);
	}
		
	destroy();
}

void destroy()
{
	close(fd); //关闭fd
	endwin();//
}

//绘制屏幕
void initUI()
{
	initscr(); //初始化curses
	//绘制窗口1,父窗口是stdscr
	winfo=derwin(stdscr,(LINES-3),COLS,0,0);

	//绘制窗口2,父窗口是stdscr
	wmsg=derwin(stdscr,3,COLS,LINES-3,0);

	//开启功能键
	keypad(stdscr,TRUE);
	keypad(wmsg,TRUE);
	keypad(winfo,TRUE);

	//画边线
	box(winfo,0,0);
	box(wmsg,0,0);

	//刷新屏幕
	refresh();
	wrefresh(winfo);
	wrefresh(wmsg);
}

//socket初始化
int initSocket()
{
	//socket初始化,生成fd
	fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd==-1) return -1;
	//构造地址结构
	dr.sin_family=AF_INET;
	dr.sin_port=htons(9989);
	dr.sin_addr.s_addr=inet_addr("192.168.64.128");
	//连接server
	r=connect(fd,(struct sockaddr*)&dr,sizeof(dr));
	if(r==-1)
	{
		close(fd);
		return -1;
	}
	return 0;
}

