/*
	ͨѶģ��
*/

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "whole.h"

// ======================= tcpSocket �� ===========================

struct tcpSocketPool
{
	int sNumber = 0;                    // ÿһ�����ӵ����
	bool isConnection = false;          // �����Ƿ�Ͽ�
	SOCKET socket = NULL;               // ���������׽���
	char clientIp[30] = { 0 };          // �ͻ��˵� IP ��ַ
	int clientPort = 0;                 // �ͻ��˵Ķ˿�
};

class tcpSocket
{
public:
	tcpSocket(const char *ip, int port);
	~tcpSocket();

public:
	bool tcpAccept();                                            // ѭ�����ܿͻ��˵����� 
	bool tcpSend(SOCKET socket, char *buf, int size);            // ����ָ�����׽��ַ�������
	bool tcpRecv(SOCKET socket, char *buf, int size);            // ����ָ�����׽��ֽ�������
	void threadToRefresh();                         // ����ˢ�¿ͻ��˵�����

private:
	void tcpClear();                                             // ����ʱ�����׽���

public:
	int maxLength = 0x1f400000;                                  // ����ͳ���,Ĭ�� 500M
};

DWORD WINAPI threadToAccept(PVOID lpParam);                      // ����ѭ�����տͻ��˵�����
DWORD WINAPI connectionHeartbeat(LPVOID lpParam);                
tcpSocketPool* connectionInfo();                                 // ���� tcpSocketPool ָ��,���ڲ鿴������Ϣ

#endif

// =================================================================