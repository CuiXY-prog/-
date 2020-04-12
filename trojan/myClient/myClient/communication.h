/*
	ͨѶģ��
*/
#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "whole.h"

class tcpSocket
{
public:
	bool tcpConnect();                                // �������ӷ����
	bool tcpSend(char* buf, int bufSize);             // ���ڸ�����˷�����Ϣ
	bool tcpRecv(char* buf, int bufSize);             // ���ڸ����ܷ���˵���Ϣ
	void clear();                                     // ������

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