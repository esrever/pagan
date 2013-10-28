#pragma once

#include <ecs/ActionMgr.h>

namespace pgn
{
	//! Stores keyboard input information
	struct cInputKeyboard
	{
		
	};

	//-------------------------------------------------------------------
	template<>
	bool from_json<cInputKeyboard>(cInputKeyboard& zObj, const rapidjson::Value& zRoot);
}