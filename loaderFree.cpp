#include "loaderFree.hpp"

void downloadFile(const char *dllLink, const char *dllPath)
{
	URLDownloadToFileA(0, dllLink, dllPath, 0, 0);
	DeleteUrlCacheEntryA(dllLink);

	std::cout << "[INFO]: software downloaded!" << std::endl;
}

bool injectDll(const char *dllPath) // by ICoded
{
	HWND hWnd = FindWindowA("cryENGINE", "Warface");
	if (!hWnd) {
		std::cout << "[ERROR]: please, start game" << std::endl;
		return false;
	}
	
	DWORD pid;
	DWORD tid = GetWindowThreadProcessId(hWnd, &pid);
	if (!tid)
	{
		std::cout << "[ERROR]: cannot get process id" << std::endl;
		return false;
	}

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, tid);
	if (!hThread || hThread == INVALID_HANDLE_VALUE)
	{
		std::cout << "[ERROR]: cannot open process" << std::endl;
		return false;
	}

	HMODULE hMod = LoadLibraryExA(dllPath, NULL, DONT_RESOLVE_DLL_REFERENCES);
	if (!hMod)
	{
		std::cout << "[ERROR]: cannot load software" << std::endl;
		return false;
	}

	if (!SetWindowsHookExW(3, (HOOKPROC)GetProcAddress, hMod, tid))
	{
		std::cout << "[ERROR]: cannot set hook" << std::endl;
		return false;
	}

	std::cout << "[INFO]: injected!" << std::endl;

	Sleep(5000);

	if (PostThreadMessageW(tid, 0, 0, 0))
	{
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
	
	return true;
}

int main()
{
	std::cout << "[INFO]: starting download..." << std::endl;
	downloadFile(DLL_LINK, DLL_PATH);

	std::cout << "[INFO]: injecting..." << std::endl;
	injectDll(DLL_PATH);

	system("pause");
	return true;
}
