/*
	�ļ�ģ��
*/

#ifndef _FILE_H_
#define _FILE_H_

#include "whole.h"

class file
{
public:
	file(const char *filePath);
	~file();

public:
	HANDLE fileRead();
	bool fileWrite(const char* buf, DWORD size);

public:
	char fileName[MAX_PATH] = { 0 };                    // ���ڱ����ļ���·��
	DWORD fileSizeH = 0;                                // ���ڱ����ļ��Ĵ�С
	DWORD fileSizeL = 0;

private:
	HANDLE hHeap  = NULL;                               // HeapCreate() ����
	HANDLE hAlloc = NULL;                               // HeapAlloc() ����
};

#endif