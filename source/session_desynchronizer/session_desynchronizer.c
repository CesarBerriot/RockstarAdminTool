#include "session_desynchronizer.h"
#include <raylib.h>
#include "game_freezer/game_freezer.h"

enum { timer_target = 10 };

static bool desynchronizing;
static float timer;

bool @is_desynchronizing_session(void)
{	return desynchronizing;
}

void @desynchronize_session(void)
{	@toggle_game_freeze();
	desynchronizing = true;
	timer = 0;
}

void @update_session_desynchronizer(void)
{	if(desynchronizing)
	{	timer += GetFrameTime();
		if(timer >= timer_target)
		{	@toggle_game_freeze();
			desynchronizing = false;
		}
	}
}

int @compute_session_desynchronizer_seconds_left(void)
{	return timer_target - timer + 1;
}
