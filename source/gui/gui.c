#include "gui.h"
#include <raylayout.h>
#include <malloc.h>
#include <make_string.h>
#include <string.h>
#include <raygui/styles/style_cyber.h>
#include "project_link_executor/project_link_executor.h"
#include "save_server_blocker/save_server_blocker.h"
#include "game_freezer/game_freezer.h"
#include "session_desynchronizer/session_desynchronizer.h"
#include "hotkey_handler/hotkey_handler.h"

static void render_button_panel(void);
static void render_game_freeze_button(void);
static void render_session_desynchronization_button(void);
static void render_save_servers_blocking_button(void);
static void render_link(void);

void @gui_run(void)
{	InitWindow(500, 300, "R* Admin Tool");
	SetTargetFPS(60);
	GuiLoadStyleCyber();
	while(!WindowShouldClose())
	{	BeginDrawing();
			ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
			GuiLayoutBeginFrame(2, RGL_PADDING_MEDIUM);
				GuiLayout(RGL_AXIS_VERTICAL, RGL_PADDING_NONE, RGL_PADDING_MEDIUM, 2, 9, 1);
					render_button_panel();
					render_link();
			GuiLayoutEndFrame();
		EndDrawing();
	}
}

static void render_button_panel(void)
{	GuiLayout(RGL_AXIS_VERTICAL, RGL_PADDING_NONE, 10, 3, 1, 1, 1);
		render_game_freeze_button();
		render_session_desynchronization_button();
		render_save_servers_blocking_button();
}

static void render_game_freeze_button(void)
{	bool disabled = @is_desynchronizing_session();
	if(disabled)
		GuiDisable();
	char * text = make_string("%sreeze Game (Ctrl + F10)", @is_game_frozen() && !@is_desynchronizing_session() ? "Unf" : "F");
	if(GuiButton(GuiLayoutNext(), text) || (!disabled && @is_hotkey_pressed(10)))
		@toggle_game_freeze();
	GuiEnable();
}

static void render_session_desynchronization_button(void)
{	if(@is_game_frozen())
		GuiDisable();
	char * text = @is_desynchronizing_session() ?
		make_string("Session Desynchronization - %i", @compute_session_desynchronizer_seconds_left()) :
		"Desync Session (Ctrl + F11)";
	if(GuiButton(GuiLayoutNext(), text) || @is_hotkey_pressed(11))
		@desynchronize_session();
	GuiEnable();

	@update_session_desynchronizer();
}

static void render_save_servers_blocking_button(void)
{	char * text = make_string("%slock R* Save Servers (Ctrl + F12)", @is_blocking_save_servers() ? "Unb" : "B");
	if(GuiButton(GuiLayoutNext(), text) || @is_hotkey_pressed(12))
		@toggle_save_server_blocking();
}

static void render_link(void)
{	if(GuiLabelButton(GuiLayoutNext(), "Copyright (c) 2025 Cesar Berriot"))
		@execute_project_link();
}
