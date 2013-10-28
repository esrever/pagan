#pragma once

#include <ecs/ActionMgr.h>

namespace pgn
{
	struct cKeyActionMapper
	{
		std::map<wchar_t, action_fun> mActions;
	};

	//-------------------------------------------------------------------
	template<>
	bool from_json<cKeyActionMapper>(cKeyActionMapper& zObj, const rapidjson::Value& zRoot);
}