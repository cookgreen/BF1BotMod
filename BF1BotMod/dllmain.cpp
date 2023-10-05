#include <Windows.h>

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	MessageBox(0, L"Hello BF1", L"BF1BotMod", 0);
	return true;
}