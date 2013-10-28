#pragma once

#include "KeyActionMapper.h"

#include <core/util/json_conversions_glm.h>
#include <core/util/jsonutils.h>

namespace pgn
{
	template<>
	bool from_json<cKeyActionMapper>(cKeyActionMapper& zObj, const rapidjson::Value& zRoot)
	{
		for(auto itr = zRoot.MemberBegin(); itr != zRoot.MemberEnd(); ++itr)
		{
			const auto& entry = *itr;
			std::string funcname = entry.name;
			
			auto keys = entry.value;
			std::vector<unsigned short> key_vals;
			if( keys.IsArray())
			{
				from_json_vector<unsigned short>(key_vals, keys);
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
		}
		return true;
	}
}