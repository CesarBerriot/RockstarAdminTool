#include "project_link_executor.h"
#include <windows.h>
#include <hard_assert.h>

void @execute_project_link(void)
{	ha_assert
	(	ShellExecuteA(NULL, NULL, "https://github.com/CesarBerriot/RockstarAdminTool", NULL, NULL, SW_SHOW) > 32,
		MODULE_NAME,
		"ShellExecuteA failure."
	);
}
