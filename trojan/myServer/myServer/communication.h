/*
	通讯模块
*/

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "whole.h"

// ======================= tcpSocket 类 ===========================

struct tcpSocketPool
{
	int sNumber = 0;                    // 每一个连接的序号
	bool isConnection = false;          // 连接是否断开
	SOCKET socket = NULL;               // 保存连接套接字
	char clientIp[30] = { 0 };          // 客户端的 IP 地址
	int clientPort = 0;                 // 客户端的端口
};

class tcpSocket
{
public:
	tcpSocket(const char *ip, int port);
	~tcpSocket();

public:
	bool tcpAccept();                                            // 循环接受客户端的请求 
	bool tcpSend(SOCKET socket, char *buf, int size);            // 根据指定的套接字发送数据
	bool tcpRecv(SOCKET socket, char *buf, int size);            // 根据指定的套接字接受数据
	void threadToRefresh();                         // 主动刷新客户端的连接

private:
	void tcpClear();                                             // 结束时清理套接字

public:
	int maxLength = 0x1f400000;                                  // 最大发送长度,默认 500M
};

DWORD WINAPI threadToAccept(PVOID lpParam);                      // 用于循环接收客户端的连接
DWORD WINAPI connectionHeartbeat(LPVOID lpParam);                
tcpSocketPool* connectionInfo();                                 // 返回 tcpSocketPool 指针,用于查看连接信息

#endif

// =================================================================