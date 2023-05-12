#include "minhook.h"
#include "hooks.h"

#include <windows.h>
#include <iostream>

namespace hooks {

	typedef HANDLE(WINAPI* CREATE_FILE_W)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
	CREATE_FILE_W p_CreateFileW = nullptr;
	CREATE_FILE_W t_CreateFileW;

	namespace anti_cheat {
		HANDLE WINAPI h_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
		{
			if (memcmp(lpFileName, L"\\\\.\\ACE-BASE", 24) == 0) {
				wprintf(L"Thread (%i) attempting to communicate with anti-cheat driver -> %s\n", GetCurrentThreadId(), lpFileName);

				SuspendThread(GetCurrentThread()); // 200iq bypass for memory protection
			}

			return p_CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		}
	}

	void create_api_hook(const wchar_t* module, const char* function, void* hookFunction, void** originalFunction, void** trampoline)
	{
		if (MH_CreateHookApiEx(module, function, hookFunction, originalFunction, trampoline) != MH_OK)
		{
			printf("Error creating hook for %s function\n", function);
			return;
		}

		if (MH_EnableHook(*trampoline) != MH_OK)
		{
			printf("Error enabling hook for %s function\n", function);
			return;
		}
	}

	void init() {
		if (MH_Initialize() != MH_OK) { puts("Error initializing MinHook library"); return; }

		hooks::create_api_hook(L"kernelbase", "CreateFileW", &anti_cheat::h_CreateFileW, reinterpret_cast<void**>(&p_CreateFileW), reinterpret_cast<void**>(&t_CreateFileW));
	}
}