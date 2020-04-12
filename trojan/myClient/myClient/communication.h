/*
	通讯模块
*/
#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "whole.h"

class tcpSocket
{
public:
	bool tcpConnect();                                // 用于连接服务端
	bool tcpSend(char* buf, int bufSize);             // 用于给服务端发送消息
	bool tcpRecv(char* buf, int bufSize);             // 用于给接受服务端的消息
	void clear();                                     // 清理函数

public:
	tcpSocket(const char* ip, u_short port);
	~tcpSocket();

private:
	SOCKET mysocket;
	sockaddr_in addr;
	WSADATA wsa = { 0 };
	int maxLength = 0x1f400000;
};

#endif