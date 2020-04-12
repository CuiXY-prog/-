#include "file.h"


/*
	储存文件名
*/
file::file(const char *filePath)
{
	memcpy(fileName, filePath, MAX_PATH);
}


/*
	清理资源
*/
file::~file()
{
	if (hHeap != NULL && hAlloc != NULL)
		::HeapFree(hHeap, 0, hAlloc);
}


/*
	读取文件,返回文件数据指针
*/
HANDLE file::fileRead()
{
	// 打开文件并保存文件的大小s
	HANDLE hFile = ::CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fileSizeL = ::GetFileSize(hFile, &fileSizeH);
		if (fileSizeL > 0x1f400000 || fileSizeH != 0)
			return false;
	}
	else
		return false;

	// 创建堆
	hHeap = ::HeapCreate(0, fileSizeL, NULL);
	if (hHeap)
	{
		hAlloc = ::HeapAlloc(hHeap, HEAP_ZERO_MEMORY, fileSizeL);
		if (!hAlloc)
			return false;
	}
	else
		return false;

	// 读取文件,返回数据的内存地址
	if (::ReadFile(hFile, hAlloc, fileSizeL, NULL, NULL))
	{
		::CloseHandle(hFile);
		return hAlloc;
	}
	return false;
}


/*
	将数据写入文件
*/
bool file::fileWrite(const char* buf, DWORD size)
{
	// 创建文件写入数据
	HANDLE hFile = ::CreateFile(fileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		if (::WriteFile(hFile, buf, size, NULL, NULL))
		{
			::CloseHandle(hFile);
			return true;
		}
	}
	else
		return false;

	return false;
}
