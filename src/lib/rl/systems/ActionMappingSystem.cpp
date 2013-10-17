#pragma once

#include "ActionMappingSystem.h"

#include <curses.h>

namespace pgn
{
	//------------------------------------------------------
	void cActionMappingSystem::Process()
	{
		chtype ch = getch();
		if(ch == 27) exit(0);
	}

	//------------------------------------------------------
	void cActionMappingSystem::from_json(const rapidjson::Value& zRoot)
	{

	}
}