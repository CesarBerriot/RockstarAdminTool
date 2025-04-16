#include "hotkey_handler.h"
#include <windows.h>

static bool is_key_down(int virtual_key_code);
static bool is_key_pressed(int virtual_key_code);

bool @is_hotkey_pressed(int function_key)
{	return (is_key_down(VK_LCONTROL) || is_key_down(VK_RCONTROL)) && is_key_pressed(VK_F1 - 1 + function_key);
}

static bool is_key_down(int virtual_key_code)
{	return GetKeyState(virtual_key_code) & 1 << 15;
}

static bool is_key_pressed(int virtual_key_code)
{	static bool previous_toggle_states[256] = { 0 };
	SHORT state = GetKeyState(virtual_key_code);
	bool down = state & 1 << (sizeof(SHORT) * 8 - 1);
	bool toggled = state & 1;
	bool result = down && toggled != previous_toggle_states[virtual_key_code];
	previous_toggle_states[virtual_key_code] = toggled;
	return result;
}
