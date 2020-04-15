#include "command.h"
#include "file.h"
#include "communication.h"

/*
struct myProtocol // ���ڿͻ��˺ͷ������˴����Э��
{
		type = 1 �����ļ����ϴ�
		type = 2 �����ļ�������
		type = 3 ��ʾ�ͻ��˻������Ƿ�׼���ô�������
		type = 12 ��ʾִ��ʧ��

int type = 0;

int fileSize = 0;
char serverFile[MAX_PATH];        // ���������ļ�
char clientFile[MAX_PATH];        // �����ϴ��ļ�
};
*/

/*
	ִ��Զ�� cmd ���� type = 4
*/
bool cmdExec(char *cmdChar, char *socketNumber, tcpSocket *socket)
{
	tcpSocketPool *pool; myProtocol cmd; HANDLE hHeap, hAlloc;
	socket->threadToRefresh();
	pool = connectionInfo();

	// �� char ����ת��Ϊ int
	int num1 = 0; int num2 = 0; int number = 0;
	num1 = *((byte *)socketNumber); num1 -= 48;
	if (strlen(socketNumber) == 2)
	{
		num2 = *((byte *)socketNumber + 1); num2 -= 48;
		number = num1 * 10 + num2;
	}
	number = num1;

	if (number >= 0 && number < 50)
	{
		if ((pool + number)->isConnection == false)
			return false;
	}
	else
		return false;

	int size = 100000;
	hHeap = HeapCreate(0, size, NULL);
	if (hHeap)
	{
		hAlloc = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, size);
		if (!hAlloc)
			return false;
	}
	else
		return false;

	cmd.type = 4;
	memcpy(cmd.cmdChar, cmdChar, MAX_PATH);
	if (socket->tcpSend((pool + number)->socket, (char*)&cmd, sizeof(cmd)))
	{
		if (socket->tcpRecv((pool + number)->socket, (char *)hAlloc, size) == false)
			return false;
	}
	else
		return false;

	cout << (char *)hAlloc << endl;
	return true;
}


/*
	�����ļ����ͻ��� type = 1
*/
bool sendToClient(char *ownFilePath, char *clientFilePath, char *socketNumber, tcpSocket *socket)
{
	tcpSocketPool *pool;
	myProtocol upload;

	socket->threadToRefresh();
	pool = connectionInfo();

	// �� char ����ת��Ϊ int
	int num1 = 0; int num2 = 0; int number = 0;
	num1 = *((byte *)socketNumber); num1 -= 48;
	if (strlen(socketNumber) == 2)
	{
		num2 = *((byte *)socketNumber + 1); num2 -= 48;
		number = num1 * 10 + num2;
	}
	number = num1;

	if (number >= 0 && number < 50)
	{
		if ((pool + number)->isConnection == false)
			return false;
	}
	else
		return false;

	// �򿪷���˵��ļ�
	file myfile(ownFilePath);
	HANDLE data = myfile.fileRead();
	if (data == false)
		return false;

	// �����ļ���С���Ҹ�֪�ͻ��˴�ŵ�·��
	upload.type = 1;
	upload.fileSize = myfile.fileSizeL;
	memcpy(upload.clientFile, clientFilePath, MAX_PATH);
	if (socket->tcpSend((pool + number)->socket, (char*)&upload, sizeof(upload)))
	{
		if (socket->tcpRecv((pool + number)->socket, (char *)&upload, sizeof(upload)) == false)
			return false;
	}
	else
		return false;

	// ����ͻ���׼�������Ϳ�ʼ�����ļ�
	if (upload.type == 3)
	{
		if (socket->tcpSend((pool + number)->socket, (char*)data, myfile.fileSizeL) == false)
			return false;
	}
	else
		return false;

	return true;
}


/*
	�������Կͻ��˵��ļ� type = 2
*/
bool recvToClient(char *ownFilePath, char *clientFilePath, char *socketNumber, tcpSocket *socket)
{
	tcpSocketPool *pool;
	myProtocol download;
	HANDLE hHeap; HANDLE hAlloc;
	bool result = true;

	// ��ȡ tcpSocketPool �ṹ��
	socket->threadToRefresh();
	pool = connectionInfo();

	// char �� int ��ת��
	int num1 = 0; int num2 = 0; int number = 0;
	num1 = *((byte *)socketNumber); num1 -= 48;
	if (strlen(socketNumber) == 2)
	{
		num2 = *((byte *)socketNumber + 1); num2 -= 48;
		number = num1 * 10 + num2;
	}
	number = num1;

	// number ������������,������ͻ��˵����Ӳ��ܶϿ�
	if (number >= 0 && number < 50)
	{
		if ((pool + number)->isConnection == false)
			return false;
	}
	else
		return false;

	// ��֪�ͻ���Ҫ�����ļ�, ���ҵȴ������ļ��Ĵ�С
	download.type = 2;
	memcpy(download.clientFile, clientFilePath, MAX_PATH);
	if (socket->tcpSend((pool + number)->socket, (char*)&download, sizeof(download)))
	{
		if (socket->tcpRecv((pool + number)->socket, (char*)&download, sizeof(download)) == false)
			return false;
	}
	else
		return false;

	// ����ͻ���׼�������������������ʹ���������׼�����տͻ��˵�����
	if (download.type == 3)
	{
		if (download.fileSize < 0 || download.fileSize > 0x1f400000)
		{
			download.type = 12;
			socket->tcpSend((pool + number)->socket, (char *)&download, sizeof(download));
			return false;
		}
	}
	else
		return false;

	hHeap = HeapCreate(0, download.fileSize, NULL);
	if (hHeap)
	{
		hAlloc = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, download.fileSize);
		if (!hAlloc)
			return false;
	}
	else
		return false;

	// ��֪�����׼������֮��Ϳ��Խ��տͻ��˵�������
	download.type = 3;
	if (socket->tcpSend((pool + number)->socket, (char *)&download, sizeof(download)))
	{
		if (socket->tcpRecv((pool + number)->socket, (char *)hAlloc, download.fileSize) == false)
			return false;
	}
	else
		return false;

	// ����ڷ���˴����ļ�
	file ownFile(ownFilePath);
	if (ownFile.fileWrite((char *)hAlloc, download.fileSize) == false)
		return false;

	HeapFree(hHeap, 0, hAlloc);
	return true;
}

