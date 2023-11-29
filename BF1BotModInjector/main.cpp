#include <Windows.h>
#include <tlhelp32.h>
#include <libloaderapi.h>
#include <iostream>

using namespace std;

void getCurrentFullPathByFile(char* full_path, const char* file_name, const char* file_ext)
{
	char szPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath_s(szPath, drive, dir, fname, ext);
	_makepath_s(full_path, MAX_PATH, drive, dir, file_name, file_ext);
}

int main(int argc, char** argv)
{
	BOOL foundBF1Process = 0;

	char bfBotModDll[MAX_PATH] = { 0 };
	getCurrentFullPathByFile(bfBotModDll, "BF1BotMod", "dll");

	HANDLE snapshot = 0; 
	DWORD exitCode = 0;

	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Process32First(snapshot, &pe32);

	do {
		//std::wcout << pe32.szExeFile << std::endl;

		if (wcscmp(pe32.szExeFile, L"bf1.exe") == 0)
		{
			foundBF1Process = 1;

			HANDLE process = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, true, pe32.th32ProcessID);
			void* lpBaseAddress = VirtualAllocEx(process, NULL, strlen(bfBotModDll), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			if (lpBaseAddress != 0)
			{
				BOOL succeededWriting = WriteProcessMemory(process, lpBaseAddress, bfBotModDll, strlen(bfBotModDll), NULL);

				HMODULE kernel32base = GetModuleHandleA("Kernel32");
				if (kernel32base != 0)
				{
					LPVOID loadLibraryAddress = (LPVOID)GetProcAddress(kernel32base, "LoadLibraryA");
					HANDLE thread = CreateRemoteThread(process, nullptr, NULL, (LPTHREAD_START_ROUTINE)loadLibraryAddress, lpBaseAddress, 0, NULL);

					if (thread != 0)
					{
						WaitForSingleObject(thread, INFINITE);
						GetExitCodeThread(thread, &exitCode);
						VirtualFreeEx(process, lpBaseAddress, 0, MEM_RELEASE);
						CloseHandle(thread);
						CloseHandle(process);
						break;
					}
					else
					{
						std::cout << "[BF1 Bot Mod Injector][Error]: The thread is invalid!";
						std::cin.get();

						return 0;
					}
				}
				else
				{
					std::cout << "[BF1 Bot Mod Injector][Error]: The kernel is invalid!";
					std::cin.get();

					return 0;
				}
			}
			else
			{
				std::cout << "[BF1 Bot Mod Injector][Error]: The base address is invalid!";
				std::cin.get();

				return 0;
			}
		}
	} while (Process32Next(snapshot, &pe32));

	if (!foundBF1Process)
	{
		cout << "[BF1 Bot Mod Injector][Error]: No any battlefield1 process found, press any key to exit." << endl;
		std::cin.get();
	}

	return 0;
}