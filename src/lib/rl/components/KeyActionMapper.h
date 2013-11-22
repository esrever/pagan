#pragma once

#include <ecs/ActionMgr.h>

namespace pgn
{
	namespace cmp
	{
		struct cKeyActionMapper
		{
			std::map<wchar_t, action_fun> mActions;
		};
	}

	//-------------------------------------------------------------------
	template<>
	bool from_json<cmp::cKeyActionMapper>(cmp::cKeyActionMapper& zObj, const rapidjson::Value& zRoot);
}