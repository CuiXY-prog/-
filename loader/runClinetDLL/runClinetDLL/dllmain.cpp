// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <Windows.h>

bool isRun = false;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		if (isRun == false)
		{
			WCHAR cmdline[MAX_PATH] = L"client.exe";
			startclient(cmdline);
		}
		isRun = true;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void startclient(WCHAR *cmdline)
{
	STARTUPINFO si = { sizeof(si) }; PROCESS_INFORMATION pi;
	CreateProcess(NULL, cmdline, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
}