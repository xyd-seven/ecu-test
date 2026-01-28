// enetsamp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <stdio.h>
#include <winsock.h>

//#include "mc_interface.h"

#define FLAG_OFF 0 // 结束标志位 OFF
#define FLAG_ON 1 // 结束标志位 ON
#define SOCK_OK 0 // 正常结束
#define SOCK_NG -1 // 异常结束
#define BUF_SIZE 4096 // 接收缓冲区大小
#define ERROR_INITIAL 0 // 初始错误
#define ERROR_SOCKET 1 // 创建 socket 错误
#define ERROR_BIND 2 // 绑定错误
#define ERROR_CONNECT 3 // 连接错误
#define ERROR_SEND 4 // 发送错误
#define ERROR_RECIEVE 5 // 接收错误
#define ERROR_SHUTDOWN 6 // 关机错误
#define ERROR_CLOSE 7 // 线路关闭错误

// 用于检查接收大小的定义
#define RECV_ANS_1 4 // 对于软元件写入的响应报文接收大小 (1E 结构 )
#define RECV_ANS_2 24　// 对于软元件读出的响应报文接收大小 (1E 结构 )

typedef struct sck_inf
{
	struct in_addr my_addr;
	unsigned short my_port;
	struct in_addr FX_IP_addr;
	unsigned short FX_port;
} SCK_INF;

static int nErrorStatus; // 错误信息 保存变量
static int Dmykeyin; // 虚拟键输入
static int Closeflag = FLAG_OFF; // 连接结束标志位
static SOCKET socketno = INVALID_SOCKET;

//explain：创建三菱mc协议所需要的tcp/ip连接及相关操作, 本进程为客户端
//para serverIP: 服务端ip地址
//para serverPort: 服务端端口号
//ret: 成功返回0，失败返回-1
int mc_open(char* serverIP, unsigned int serverPort)
{
	WORD wVersionRequested = MAKEWORD(1, 1); // Winsock Ver 1.1 要求
	WSADATA wsaData;
	struct sck_inf sc;
	struct sockaddr_in hostdata; // 对方设备侧数据
	struct sockaddr_in FX3UENET_L; // 以太网模块侧数据
	void Sockerror(int); // 错误处理函数
	unsigned long ulCmdArg ; // 非阻塞模式设置标志位

	sc.my_addr.s_addr = htonl(INADDR_ANY); // 对方设备侧 IP 地址
	sc.my_port = htons(0); // 对方设备侧端口号
	sc.FX_IP_addr.s_addr = inet_addr(serverIP); // 以太网模块侧 IP 地址

	sc.FX_port = htons(serverPort); // 以太网模块侧端口号
	Closeflag = FLAG_OFF; // 连接结束标志位 OFF
	nErrorStatus = WSAStartup(wVersionRequested, &wsaData); // Winsock 初始处理
	if(nErrorStatus != SOCK_OK) 
	{
		Sockerror(ERROR_INITIAL); // 错误处理
		return (SOCK_NG);
	}
	printf("Winsock Version is %1d.%1d\n", HIBYTE(wsaData.wVersion), LOBYTE(wsaData.wVersion));
	socketno = socket(AF_INET, SOCK_STREAM, 0); // 创建 TCP/IP 的套接字
	if(socketno == INVALID_SOCKET) 
	{
		Sockerror(ERROR_SOCKET); // 错误处理
		return (SOCK_NG);
	}
	Closeflag = FLAG_ON; // 连接结束标志位 ON

	hostdata.sin_family = AF_INET;
	hostdata.sin_port = sc.my_port;
	hostdata.sin_addr.s_addr = sc.my_addr.s_addr;
	if(bind(socketno, (LPSOCKADDR)&hostdata, sizeof(hostdata)) != SOCK_OK) 
	{
		// 绑定
		Sockerror(ERROR_BIND); // 错误处理
		return (SOCK_NG);
	}
	FX3UENET_L.sin_family = AF_INET;
	FX3UENET_L.sin_port = sc.FX_port;
	FX3UENET_L.sin_addr.s_addr = sc.FX_IP_addr.s_addr;
	if(connect(socketno, (LPSOCKADDR)&FX3UENET_L, sizeof(FX3UENET_L)) != SOCK_OK) 
	{
		// 连接 (Active 打开 )
		Sockerror(ERROR_CONNECT); // 错误处理
		return (SOCK_NG);
	}
	//Closeflag = FLAG_ON; // 连接结束标志位 ON
	// 设为非阻塞模式
	ulCmdArg = 1;
	ioctlsocket(socketno, FIONBIO, &ulCmdArg); // 设置为非阻塞模式

	return 0;
}

//explain：关闭三菱mc协议所需要的tcp/ip连接及相关操作
//ret: 成功返回0，失败返回-1
int mc_close(void)
{
	if (Closeflag == FLAG_ON)
	{
		nErrorStatus = shutdown(socketno, 2); // 关机处理
		nErrorStatus = closesocket(socketno); // 关闭处理
		Closeflag = FLAG_OFF; // 连接结束标志位 OFF
	}
	//printf(" 结束程序。请按任意键。 \n");
	//Dmykeyin = getchar(); // 等待键输入
	WSACleanup(); // Winsock.DLL 释放

	return 0;
}

//explain：读取M区软元件状态
//para addr: 软元件地址, 开始软元件M1000地址为1000
//para timeout: 读取超时时间，0 无限等待; 1~65535等待时间(单位250ms); 一般使用10(2.5s)
//ret: 成功返回软元件状态,0或者1; 失败返回-1
int mc_read(unsigned int addr, unsigned int timeout)
{
	int i = 0;
	int length; // 通信数据长度
	int rbuf_idx; // 接收数据保存起始变
	int recv_size; // 接收数据个数
	unsigned char s_buf[BUF_SIZE]; // 发送缓冲区
	unsigned char r_buf[BUF_SIZE]; // 接收缓冲区

	//对于软元件读出
	//example: 00FF000A4D20000003E80100
	sprintf((char*)s_buf, "00FF%04X4D200000%04X0100", timeout, addr);
	length = (int)strlen((const char *)s_buf);
	if (send(socketno, (const char *)s_buf, length, 0) == SOCKET_ERROR) 
	{
		nErrorStatus = WSAGetLastError();
		printf("mc_read: snd fail, err type[%d].\n", nErrorStatus);
		return -1;
	}
	printf("\n mc_read: 发送数据 \n%s\n", s_buf);
	// 在检查接收大小的同时执行接收处理
	rbuf_idx = 0; // 接收数据保存起始变址初始化
	recv_size = 0; // 初始化接收数据数

	while (1) 
	{
		length = recv(socketno, (char *)&r_buf[rbuf_idx], (BUF_SIZE - rbuf_idx), 0);

		if (length == 0) 
		{ 
			// 连接已被切断？
			nErrorStatus = WSAGetLastError();
			printf("mc_read: rcv fail, err type[%d].\n", nErrorStatus);
			return -1;
		}

		if(length == SOCKET_ERROR) 
		{
			nErrorStatus = WSAGetLastError();
			if(nErrorStatus != WSAEWOULDBLOCK) 
			{
				nErrorStatus = WSAGetLastError();
				printf("mc_read: rcv fail 2, err type[%d].\n", nErrorStatus);
				return -1;
			} 
			else 
			{
				continue; // 重复直至可以接收
			}
		} 
		else 
		{
			rbuf_idx += length; // 更新接收数据保存位置
			recv_size += length; // 更新接收数据数
			if(recv_size >= 6) // 已接收了全部的响应报文？
				break; // 已接收，停止重复
		}
	}
	r_buf[rbuf_idx] = '\0'; //在接收数据的末尾
	
	//设置NULL
	printf("\n mc_read: 接收数据 \n%s\n", r_buf);
	/*printf("\n");
	for (i = 0; i < rbuf_idx; i++)
	{
		printf("mc_read: data%d, [%d].\n", i, r_buf[i]-'0');
	}
	printf("\n");*/

	return r_buf[4]-'0';
}

//explain：写M区软元件状态
//para addr: 软元件地址, 开始软元件M1000地址为1000; 结束软元件M1050地址为1050; 测试结果软元件为M1055~M1058
//para state: 0 or 1 
//para timeout: 写入超时时间，0 无限等待; 1~65535等待时间(单位250ms); 一般使用10(2.5s)
//ret: 写入成功返回0; 失败返回-1
int mc_write(unsigned int addr, unsigned char state, unsigned int timeout)
{
	int i = 0;
	int length; // 通信数据长度
	int rbuf_idx; // 接收数据保存起始变
	int recv_size; // 接收数据个数
	unsigned char s_buf[BUF_SIZE]; // 发送缓冲区
	unsigned char r_buf[BUF_SIZE]; // 接收缓冲区

	//对于软元件写入
	//M1000 example: 02FF000A4D20000003E8010000
	//M1055 example: 02FF000A4D200000041F010010
	//M1056 example: 02FF000A4D2000000420010010
	//M1057 example: 02FF000A4D2000000421010010
	//M1058 example: 02FF000A4D2000000422010010
	//M1050 example: 02FF000A4D200000041A010010
	sprintf((char *)s_buf, "02FF%04X4D200000%04X0100%X0", timeout, addr, state);
	length = (int)strlen((const char *)s_buf);
	if (send(socketno, (const char *)s_buf, length, 0) == SOCKET_ERROR) 
	{
		nErrorStatus = WSAGetLastError();
		printf("mc_write: snd fail, err type[%d].\n", nErrorStatus);
		return -1;
	}
	printf("\n mc_write: 发送数据 \n%s\n", s_buf);
	// 在检查接收大小的同时执行接收处理
	rbuf_idx = 0; // 接收数据保存起始变址初始化
	recv_size = 0; // 初始化接收数据数

	while (1) 
	{
		length = recv(socketno, (char *)&r_buf[rbuf_idx], (BUF_SIZE - rbuf_idx), 0);

		if (length == 0) 
		{ 
			// 连接已被切断？
			nErrorStatus = WSAGetLastError();
			printf("mc_write: rcv fail, err type[%d].\n", nErrorStatus);
			return -1;
		}

		if(length == SOCKET_ERROR) 
		{
			nErrorStatus = WSAGetLastError();
			if(nErrorStatus != WSAEWOULDBLOCK) 
			{
				nErrorStatus = WSAGetLastError();
				printf("mc_write: rcv fail 2, err type[%d].\n", nErrorStatus);
				return -1;
			} 
			else 
			{
				continue; // 重复直至可以接收
			}
		} 
		else 
		{
			rbuf_idx += length; // 更新接收数据保存位置
			recv_size += length; // 更新接收数据数
			if (recv_size >= 4) // 已接收了全部的响应报文？
				break; // 已接收，停止重复
		}
	}
	r_buf[rbuf_idx] = '\0'; //在接收数据的末尾
	
	//设置NULL
	printf("\n mc_write: 接收数据 \n%s\n", r_buf);
	/*printf("\n");
	for (i = 0; i < rbuf_idx; i++)
	{
		printf("mc_write: data%d, [%d].\n", i, r_buf[i]-'0');
	}
	printf("\n");*/

	return 0;
}

void Sockerror(int error_kind) // 错误处理函数
{
	if (error_kind == ERROR_INITIAL)
	{
		printf(" 初始处理异常。");
	}
	else
	{
		nErrorStatus = WSAGetLastError();
		switch(error_kind)
		{
			case ERROR_SOCKET:
							  printf(" 未能创建 socket。");
							  break;
			case ERROR_BIND:
							  printf(" 未能绑定。");
							  break;
			case ERROR_CONNECT:
							  printf(" 未能确立连接。");
							  break;
			case ERROR_SEND:
							  printf(" 未能发送。");
							  break;
			case ERROR_RECIEVE:
							  printf(" 未能接收。");
							  break;
			case ERROR_SHUTDOWN:
							  printf(" 未能 Shutdown。");
							  break;
			case ERROR_CLOSE:
							  printf(" 未能正常关闭。");
							  break;
		}
	}

	printf(" 错误代码为 %d 。 \n", nErrorStatus);
	if (Closeflag == FLAG_ON)
	{
		nErrorStatus = shutdown(socketno, 2); // 关机处理
		nErrorStatus = closesocket(socketno); // 关闭处理
		Closeflag = FLAG_OFF; // 连接结束标志位 OFF
	}
	//printf(" 结束程序。请按任意键。 \n");
	//Dmykeyin = getchar(); // 等待键输入
	WSACleanup(); // Winsock.DLL 释放
	return;
}
#if 0
//for test
int main(void)
{
	int ret = 0;

	//if (mc_open("192.168.1.151", 11004) == 0)
	if (mc_open("192.168.1.91", 3050) == 0)
	{
		//printf("\ntest1:*****************\n");
		ret = mc_read(1600, 10);
		printf("mc_read ret[%d].\n\n", ret);
		Sleep(5*1000);

		//printf("\ntest2:*****************\n");
		ret = mc_write(1600, 1, 10);
		printf("mc_write 1 ret[%d].\n\n", ret);
		Sleep(5*1000);

		//printf("\ntest3:*****************\n");
	    ret = mc_read(1600, 10);
		printf("mc_read ret[%d].\n\n", ret);
		Sleep(5*1000);
        
		///*
		//printf("\ntest4:*****************\n");
		ret = mc_write(1600, 0, 10);
		printf("mc_write 0 ret[%d].\n\n", ret);
		Sleep(5*1000);

		//printf("\ntest5:*****************\n");
	    ret = mc_read(1600, 10);
		printf("mc_read ret[%d].\n\n", ret);
		Sleep(5*1000);
		//*/

		mc_close();
	}

	printf(" 结束程序。请按任意键。 \n");
	Dmykeyin = getchar(); // 等待键输入
	return 0;
}
#endif








