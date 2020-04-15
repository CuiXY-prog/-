#include "main.h"

/*
	������
*/
int main(int argc, char **argv)
{
	tcpSocket socket("192.168.1.3", 5809);     // ����˿�ʼ�������ҵȴ��ͻ��˵�����
	if (!socket.tcpAccept())                 
		return false;

	commandReduce(&socket);                    // ��ʼ�����еĽ���
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





