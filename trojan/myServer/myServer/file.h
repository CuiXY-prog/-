/*
	文件模块
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
	char fileName[MAX_PATH] = { 0 };                    // 用于保存文件的路径
	DWORD fileSizeH = 0;                                // 用于保存文件的大小
	DWORD fileSizeL = 0;

private:
	HANDLE hHeap  = NULL;                               // HeapCreate() 返回
	HANDLE hAlloc = NULL;                               // HeapAlloc() 返回
};

#endif