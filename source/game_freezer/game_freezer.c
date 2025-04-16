#include "game_freezer.h"
#include <stdbool.h>
#include <windows.h>
#include <hard_assert.h>
#include <make_string.h>

typedef LONG(NTAPI*signature_t)(HANDLE ProcessHandle);

static HWND find_game_window(void);
static void call_function(char name[]);

bool @is_game_frozen(void)
{	DWORD game_window_thread_identifier = GetWindowThreadProcessId(find_game_window(), NULL);
	ha_assert(game_window_thread_identifier, MODULE_NAME, "GetWindowThreadProcessId failure.");

	HANDLE game_window_thread = OpenThread(THREAD_SUSPEND_RESUME, false, game_window_thread_identifier);
	ha_assert(game_window_thread, MODULE_NAME, "OpenThread failure.");

	DWORD suspension_count = SuspendThread(game_window_thread);
	ha_assert(suspension_count != -1, MODULE_NAME, "SuspendThread failure.");

	ha_assert(ResumeThread(game_window_thread) != -1, MODULE_NAME, "ResumeThread failure.");

	ha_assert(CloseHandle(game_window_thread), MODULE_NAME, "OpenThread failure.");

	return suspension_count;
}

void @toggle_game_freeze(void)
{	call_function(@is_game_frozen() ? "NtResumeProcess" : "NtSuspendProcess");
}

static HWND find_game_window(void)
{	HWND game_window = FindWindowA(NULL, "Grand Theft Auto V");
	ha_assert(game_window, MODULE_NAME, "Couldn't find the game window.");
	return game_window;
}

static void call_function(char name[])
{	HMODULE module = GetModuleHandleA("ntdll.dll");
	ha_assert(module, MODULE_NAME, "GetModuleHandleA failure.");

	FARPROC address = GetProcAddress(module, name);
	ha_assert(module, MODULE_NAME, "GetProcAddress failure.");

	DWORD game_process_identifier;
	ha_assert(GetWindowThreadProcessId(find_game_window(), &game_process_identifier), MODULE_NAME, "GetWindowThreadProcessId failure.");

	HANDLE game_process = OpenProcess(PROCESS_SUSPEND_RESUME, false, game_process_identifier);
	ha_assert(game_process, MODULE_NAME, "OpenProcess failure.");

	ha_assert(!(signature_t)address(game_process), MODULE_NAME, make_string("%s failure.", name));

	ha_assert(CloseHandle(game_process), MODULE_NAME, "CloseHandle failure.");
}
