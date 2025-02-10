#include "includes.h"

namespace console
{
	FILE* output_stream = nullptr;

	void attach(const char *name)
	{
		if (AllocConsole())
		{
			freopen_s(&output_stream, "conout$", "w", stdout);
		}
		SetConsoleTitleA(name);
	}

	void detach()
	{
		if (output_stream)
		{
			fclose(output_stream);
		}
		FreeConsole();
	}
}

#define RAISE_ERROR(check_var, error_message, success_message) \
if (!check_var) \
{ \
	MessageBoxA(NULL, error_message, "BF1 Bot Mod", MB_OK | MB_ICONERROR); \
	FreeLibraryAndExitThread(globals::hmModule, 1); \
} \
else \
{ \
	std::cout << success_message << "0x" << std::hex << check_var << std::endl; \
} \

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	auto process_id_that_interests_us_very_much = GetCurrentProcessId();

	HWND* cur_hwnd = (HWND*)lParam;

	if ((!GetWindow(hwnd, GW_OWNER)) && IsWindow(hwnd))
	{
		DWORD process_id = NULL;
		GetWindowThreadProcessId(hwnd, &process_id);

		char* text_window = new char[255];

		GetWindowTextA(hwnd, text_window, 255);

		if (process_id_that_interests_us_very_much == process_id && strstr(text_window, "Battlefield") && !strstr(text_window, ".exe"))
		{
			std::cout << "Window: " << text_window << std::endl;
			*cur_hwnd = hwnd;
			return 0;
		}
	}

	return 1;
}

void SetupBotModThread(void)
{
	//allocate console
	console::attach("BF1 Bot Mod");
}

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:    
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetupBotModThread, NULL, NULL, NULL);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

