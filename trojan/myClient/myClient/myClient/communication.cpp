#include "communication.h"

tcpSocket::tcpSocket(const char* ip, u_short port)
{
	addr.sin_family = AF_INET;                  
	addr.sin_port   = ::htons(port);
	addr.sin_addr.S_un.S_addr = ::inet_addr(ip);
}

tcpSocket::~tcpSocket()
{
	clear();
}

bool tcpSocket::tcpConnect()
{
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) == NO_ERROR)
	{
		mysocket = ::socket(AF_INET, SOCK_STREAM, 0);
		if (mysocket == INVALID_SOCKET)
		{
			WSACleanup();
			return false;
		}
	}
	else
		return false;

	if (::connect(mysocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		return false;

	return true;
}

void tcpSocket::clear()
{
	if (mysocket != INVALID_SOCKET && mysocket != NULL)
		::closesocket(mysocket);
	::WSACleanup();
}

bool tcpSocket::tcpSend(char* buf, int bufSize)
{
	if (bufSize > 0x1f400000) // 支持最多 500M 的文件
		return false;

	int lastLength = 0;
	if (maxLength != 0 && bufSize > maxLength)
	{
		for (int i = 0; i < (bufSize / maxLength); i++)
		{
			if (send(mysocket, buf, maxLength, 0) == SOCKET_ERROR)
				return false;
			buf += maxLength;
		}
		lastLength = bufSize - (bufSize / maxLength) * maxLength;
		if (lastLength > 0)
		{
			if (send(mysocket, buf, lastLength, 0) == SOCKET_ERROR)
				return false;
		}
	}
	if (send(mysocket, buf, bufSize, 0) == SOCKET_ERROR)
		return false;

	return true;
}

bool tcpSocket::tcpRecv(char* buf, int bufSize)
{
	if (bufSize > 0x1f400000) // 支持最多 500M 的文件
		return false;

	int lastLength = 0; int res = 0;
	if (maxLength != 0 && bufSize > maxLength)
	{
		for (int i = 0; i < (bufSize / maxLength); i++)
		{
			res = recv(mysocket, buf, maxLength, 0);
			if (res == SOCKET_ERROR || res == 0)
				return false;
		}
		lastLength = bufSize - (bufSize / maxLength) * maxLength;
		if (lastLength > 0)
		{
			res = recv(mysocket, buf, lastLength, 0);
			if (res == SOCKET_ERROR || res == 0)
				return false;
		}
	}

	res = recv(mysocket, buf, bufSize, 0);
	if (res == SOCKET_ERROR || res == 0)
		return false;

	return true;
}