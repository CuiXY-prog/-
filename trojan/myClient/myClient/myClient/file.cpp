#include "file.h"

file::file(const char* filePath)
{
	memcpy(fileName, filePath, MAX_PATH);
}

file::~file()
{
	if (hHeap != NULL && hAlloc != NULL)
		::HeapFree(hHeap, 0, hAlloc);
}

HANDLE file::fileRead()
{
	HANDLE hFile = ::CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fileSizeL = ::GetFileSize(hFile, &fileSizeH);
		if (fileSizeL > 0x1f400000 || fileSizeH != 0)
			return false;
	}
	else
		return false;

	hHeap = ::HeapCreate(0, fileSizeL, NULL);
	if (hHeap)
	{
		hAlloc = ::HeapAlloc(hHeap, HEAP_ZERO_MEMORY, fileSizeL);
		if (hAlloc == NULL)
			return false;
	}
	else
		return false;

	if (::ReadFile(hFile, hAlloc, fileSizeL, NULL, NULL))
	{
		::CloseHandle(hFile);
	}
	else
		return false;

	return hAlloc;
}

bool file::fileWrite(const char* buf, DWORD size)
{
	HANDLE hFile = ::CreateFile(fileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		if (::WriteFile(hFile, buf, size, NULL, NULL) == false)
			return false;
	}
	else
		return false;

	::CloseHandle(hFile);
	return true;
}