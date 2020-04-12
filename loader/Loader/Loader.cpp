#include "Loader.h"
#include "selfStart.h"

bool RunExe(char *filePath)
{
	STARTUPINFO si = { sizeof(si) }; PROCESS_INFORMATION pi;
	if (CreateProcess(NULL, filePath, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi) == false)
	{
		return false;
	}
	return true;
}

int main(int argc, char **argv)
{
	char filePath[MAX_PATH] = "client.exe";
	selfStarting();
	RunExe(filePath);
	return 0;
}