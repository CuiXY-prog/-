#include "communication.h"

sockaddr_in addr;                   // ��ʼ�� ip �Ͷ˿�
WSADATA wsa = { 0 };                // ���� WSAStartup()
SOCKET mysocket = NULL;             // socket() ����
tcpSocketPool pool[50];             // �������ͻ��˵�������Ϣ

/*
	���ڳ�ʼ�� sockaddr_in ���Ҹ� tcpSocketPool �������
*/
tcpSocket::tcpSocket(const char *ip, int port)
{
	addr.sin_family = AF_INET;                            // ��ʼ�� sockaddr_in �ṹ��
	addr.sin_port = ::htons(port);
	addr.sin_addr.S_un.S_addr = ::inet_addr(ip);

	for (int i = 0; i < 50; i++)                          // Ϊ tcpSocketPool �����������
		pool[i].sNumber = i;
}

/*
	���� tcpSocket �������Դ
*/
tcpSocket::~tcpSocket()
{
	tcpClear();
}


/*
	ʵ�ַ���˵Ķ˿ڰ󶨲������ͻ��˵�����
*/
bool tcpSocket::tcpAccept()
{
	// �����׽���
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

	// �󶨶˿ڲ�����
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

	// ����һ���߳�ȥ���տͻ��˵�����
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
				pool[i].socket = acceptSocket;   // ���ͻ��˵Ĳ�����Ϣ������ pool ��
				pool[i].isConnection = true;
				pool[i].clientPort = ntohs(addr.sin_port);
				ip = inet_ntoa(addr.sin_addr);
				memcpy(pool[i].clientIp, ip, strlen(ip));
				break;
			}
			if (i == 49 && pool[i].isConnection == true)  // ��� pool ������ô�Ͽ�����
				::closesocket(acceptSocket);
		}
	}
	return true;
}


/*
	ͨ��ָ���Ŀͻ������ӽ�������
*/
bool tcpSocket::tcpRecv(SOCKET socket, char *buf, int size)
{
	// ֧����� 500M ���ļ�
	if (size > 0x1f400000)
		return false;

	// ����������ֶν���
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

	// һ���Խ�������
	result = ::recv(socket, buf, size, 0);
	if (result == SOCKET_ERROR || result == 0)
		return false;

	return true;
}


/*
	ͨ��ָ���Ŀͻ������ӷ�������
*/
bool tcpSocket::tcpSend(SOCKET socket, char *buf, int size)
{
	// ֧����� 500M ���ļ�
	if (size > 0x1f400000)
		return false;

	// ����������ֶη���
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

	// һ���Է���
	if (::send(socket, buf, size, 0) == SOCKET_ERROR)
		return false;

	return true;
}


/*
	������
*/
void tcpSocket::tcpClear()
{
	if (mysocket != INVALID_SOCKET && mysocket != NULL)
		::closesocket(mysocket);
	::WSACleanup();
}


/*
	���ڷ��� tcpSocketPool �ṹָ��, ����鿴�ͻ��˵��������
*/
tcpSocketPool* connectionInfo()
{
	tcpSocketPool *result = pool;
	return result;
}


/*
	����ˢ�¿ͻ��˵��������
*/
void tcpSocket::threadToRefresh()
{
	static int paramId[50];
	for (int i = 0; i < 50; i++)
	{
		paramId[i] = i;
	}

	// �����̲߳��Կͻ��˵�����
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

	// �����߳��ͷ���Դ
	for (int i = 0; i <= index; i++) 
		::TerminateThread(threadHandles[i], 0);
}

DWORD WINAPI connectionHeartbeat(LPVOID lpParam)
{
	//  ��� recv = 0 ���ʾ�ͻ������ӶϿ�
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