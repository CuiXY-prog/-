/*
	���ļ�ͷ
*/
#define _CRT_SECURE_NO_WARNINGS
#ifndef _WHOLE_H_
#define _WHOLE_H_

#include <Windows.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
using namespace std;

struct myProtocol // ���ڿͻ��˺ͷ������˴����Э��
{
	/*
		type = 1 �����ļ����ϴ�
		type = 2 �����ļ�������
		type = 3 ��ʾ�ͻ��˻������Ƿ�׼���ô�������
		type = 4 ��ʾ����ִ��
		...
	*/
	int type = 0;

	/*
		type = 1 && type = 2
	*/
	int fileSize = 0;
	char clientFile[MAX_PATH] = { 0 };        // �����ϴ��ļ�

	/*
		type = 4
	*/
	char cmdChar[MAX_PATH] = { 0 };           // ��������ִ��
};

#endif