#include "communication.h"

sockaddr_in addr;                   // 初始化 ip 和端口
WSADATA wsa = { 0 };                // 用于 WSAStartup()
SOCKET mysocket = NULL;             // socket() 返回
tcpSocketPool pool[50];             // 保存多个客户端的连接信息

/*
	用于初始化 sockaddr_in 并且给 tcpSocketPool 标明序号
*/
tcpSocket::tcpSocket(const char *ip, int port)
{
	addr.sin_family = AF_INET;                            // 初始化 sockaddr_in 结构体
	addr.sin_port = ::htons(port);
	addr.sin_addr.S_un.S_addr = ::inet_addr(ip);

	for (int i = 0; i < 50; i++)                          // 为 tcpSocketPool 标明连接序号
		pool[i].sNumber = i;
}

/*
	清理 tcpSocket 申请的资源
*/
tcpSocket::~tcpSocket()
{
	tcpClear();
}


/*
	实现服务端的端口绑定并监听客户端的连接
*/
bool tcpSocket::tcpAccept()
{
	// 创建套接字
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) == NO_ERROR)
	{
		mysocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (mysocket == INVALID_SOCKET)
		{
			::WSACleanup();
			return false;
		}
	}
	else
		return false;

	// 绑定端口并监听
	if (::bind(mysocket, (SOCKADDR*)&addr, sizeof(addr)) != SOCKET_ERROR)
	{
		if (::listen(mysocket, 5) == SOCKET_ERROR)
			return false;
	}
	else
	{
		tcpClear();
		return false;
	}

	// 创建一个线程去接收客户端的连接
	HANDLE hAccept  = ::CreateThread(NULL, 0, threadToAccept,  0, 0, 0);
	WaitForSingleObject(hAccept, 0);
	return true;
}

DWORD WINAPI threadToAccept(PVOID lpParam)
{
	sockaddr_in addr = { 0 }; int addrSize = sizeof(sockaddr_in);
	char *ip = NULL; SOCKET acceptSocket = NULL;

	while (true)
	{
		acceptSocket = ::accept(mysocket, (sockaddr *)&addr, &addrSize);
		for (int i = 0; i < 50; i++)
		{
			if (pool[i].isConnection == false)
			{
				pool[i].socket = acceptSocket;   // 将客户端的部分信息储存在 pool 中
				pool[i].isConnection = true;
				pool[i].clientPort = ntohs(addr.sin_port);
				ip = inet_ntoa(addr.sin_addr);
				memcpy(pool[i].clientIp, ip, strlen(ip));
				break;
			}
			if (i == 49 && pool[i].isConnection == true)  // 如果 pool 满了那么断开连接
				::closesocket(acceptSocket);
		}
	}
	return true;
}


/*
	通过指定的客户端连接接收数据
*/
bool tcpSocket::tcpRecv(SOCKET socket, char *buf, int size)
{
	// 支持最多 500M 的文件
	if (size > 0x1f400000)
		return false;

	// 满足条件则分段接收
	int lastLength = 0; int result;
	if (maxLength != 0 && size > maxLength)
	{
		for (int i = 0; i < (size / maxLength); i++)
		{
			result = ::recv(socket, buf, maxLength, 0);
			if (result == SOCKET_ERROR || result == 0)
				return false;
		}
		lastLength = size - (size / maxLength) * maxLength;
		if (lastLength > 0)
		{
			result = ::recv(socket, buf, lastLength, 0);
			if (result == SOCKET_ERROR || result == 0)
				return false;
		}
	}

	// 一次性接收数据
	result = ::recv(socket, buf, size, 0);
	if (result == SOCKET_ERROR || result == 0)
		return false;

	return true;
}


/*
	通过指定的客户端连接发送数据
*/
bool tcpSocket::tcpSend(SOCKET socket, char *buf, int size)
{
	// 支持最多 500M 的文件
	if (size > 0x1f400000)
		return false;

	// 满足条件则分段发送
	int lastLength = 0;
	if (maxLength != 0 && size > maxLength)
	{
		for (int i = 0; i < (size / maxLength); i++)
		{
			if (::send(socket, buf, maxLength, 0) == SOCKET_ERROR)
				return false;
			buf += maxLength;
		}
		lastLength = size - (size / maxLength) * maxLength;
		if (lastLength > 0)
		{
			if (::send(socket, buf, lastLength, 0) == SOCKET_ERROR)
				return false;
		}
	}

	// 一次性发送
	if (::send(socket, buf, size, 0) == SOCKET_ERROR)
		return false;

	return true;
}


/*
	清理函数
*/
void tcpSocket::tcpClear()
{
	if (mysocket != INVALID_SOCKET && mysocket != NULL)
		::closesocket(mysocket);
	::WSACleanup();
}


/*
	用于返回 tcpSocketPool 结构指针, 方便查看客户端的连接情况
*/
tcpSocketPool* connectionInfo()
{
	tcpSocketPool *result = pool;
	return result;
}


/*
	主动刷新客户端的连接情况
*/
void tcpSocket::threadToRefresh()
{
	static int paramId[50];
	for (int i = 0; i < 50; i++)
	{
		paramId[i] = i;
	}

	// 创建线程测试客户端的连接
	HANDLE threadHandles[50]; int index = 0;
	for (int i = 0; i < 50; i++)
	{
		if (pool[i].isConnection == true)
		{
			int param = i;
			threadHandles[index] = ::CreateThread(NULL, 0, connectionHeartbeat, (LPVOID)&paramId[i], 0, 0);
			index += 1;
		}
	}
	::WaitForMultipleObjects(index, threadHandles, TRUE, 500); Sleep(500);

	// 销毁线程释放资源
	for (int i = 0; i <= index; i++) 
		::TerminateThread(threadHandles[i], 0);
}

DWORD WINAPI connectionHeartbeat(LPVOID lpParam)
{
	//  如果 recv = 0 则表示客户端连接断开
	int i = *((int *)(lpParam));
	myProtocol recvData; int result;
	result = ::recv(pool[i].socket, (char *)&recvData, sizeof(myProtocol), 0);
	
	if (result == 0 || result == SOCKET_ERROR)
	{
		pool[i].isConnection = false;
		ZeroMemory(pool[i].clientIp, 30);
		return true;
	}
	return false;
}