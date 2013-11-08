#pragma once

#include <vector>
#include <string>

#include <core/util/json_conversions.h>
#include "ecs_config.h"

namespace pgn
{
	struct cArchetype
	{
		std::string				 mName;
		component_mask_type		 mMask;
		std::vector<std::string> mTags;
		
	};

	//---------------------------------------------
	template<>
	inline void to_json<cArchetype>(const cArchetype& zObj, JsonWriter& writer)
	{
		writer.StartObject();
		JsonWriter_AddMember("Name", zObj.mName, writer);
		JsonWriter_AddMember("Mask", zObj.mMask, writer);
		JsonWriter_AddMember("Tags", zObj.mTags, writer);
		writer.EndObject();
	}
}