#pragma once

#include "ActionMappingSystem.h"

#include <curses.h>

#include <rl/events/events.h>

namespace pgn
{
	//------------------------------------------------------
	void cActionMappingSystem::Process()
	{
		chtype ch = getch();
		if(ch == 27) cExitApplicationEventData::emit();
	}

	//------------------------------------------------------
	void cActionMappingSystem::from_json(const rapidjson::Value& zRoot)
	{
		assert(false);
	}
}