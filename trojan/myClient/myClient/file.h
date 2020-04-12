/*
	�ļ�ģ��
*/
#ifndef _FILE_H_
#define _FILE_H_

#include "whole.h"

class file
{
public:
	file(const char* filePath);
	~file();

public:
	HANDLE fileRead();
	bool fileWrite(const char* buf, DWORD size);

public:
	char fileName[MAX_PATH] = { 0 };                        // �ļ�·��
	DWORD fileSizeH = 0;                                      // �ļ���С
	DWORD fileSizeL = 0;
	HANDLE hHeap = NULL;                                    // return for HeapCreate()
	HANDLE hAlloc = NULL;                                   // return for HeapAlloc()
};

#endif
