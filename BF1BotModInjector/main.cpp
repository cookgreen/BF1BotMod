#include <Windows.h>
#include <tlhelp32.h>
#include <libloaderapi.h>

void make_full_path(char* full_path, const char* file_name, const char* file_ext)
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
	char dll_full_path[MAX_PATH];
	make_full_path(dll_full_path, "BF1BotMod", "dll");

	HANDLE snapshot = 0; 
	DWORD exitCode = 0;

	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Process32First(snapshot, &pe32);

	do {
		if (wcscmp(pe32.szExeFile, L"bf1.exe") == 0)
		{
			HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, true, pe32.th32ProcessID);
			void* lpBaseAddress = VirtualAllocEx(process, NULL, strlen(dll_full_path) + 1, MEM_COMMIT, PAGE_READWRITE);
			WriteProcessMemory(process, lpBaseAddress, dll_full_path, strlen(dll_full_path) + 1, NULL);
			
			HMODULE kernel32base = GetModuleHandle(L"kernel32.dll");
			
			HANDLE thread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(kernel32base, "LoadLibraryA"), lpBaseAddress, 0, NULL);
			
			WaitForSingleObject(thread, INFINITE);
			GetExitCodeThread(thread, &exitCode);

			VirtualFreeEx(process, lpBaseAddress, 0, MEM_RELEASE);
			CloseHandle(thread);
			CloseHandle(process);
			break;
		}
	} while (Process32Next(snapshot, &pe32));

	return 0;
}