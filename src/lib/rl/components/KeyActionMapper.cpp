#pragma once

#include "KeyActionMapper.h"
#include <core/util/jsonutils.h>

#include <ecs/ecs.h>
#include <ecs/ActionMgr.h>

namespace pgn
{
	template<>
	bool from_json<cKeyActionMapper>(cKeyActionMapper& zObj, const rapidjson::Value& zRoot)
	{
		for(auto itr = zRoot.MemberBegin(); itr != zRoot.MemberEnd(); ++itr)
		{
			const auto& entry = *itr;
			std::string funcname = entry.name.GetString();
			
			const auto& keys = entry.value;
			std::vector<unsigned short> key_vals;
			if( keys.IsArray())
			{
				read_json_vector(key_vals, keys);
			}
			else if(keys.IsObject())
			{
				unsigned short key_val;
				if( from_json(key_val, keys))
					key_vals.push_back(key_val);
			}
			else
			{
				assert(false);
			}

			if(!key_vals.empty())
			{
				auto& amgr = ECS.mActionMgr;
				auto fun = amgr->GetAction(funcname);
				if(fun)
					for(auto key_val : key_vals)
						zObj.mActions[key_val] = fun;
			}
		}
		return true;
	}
}