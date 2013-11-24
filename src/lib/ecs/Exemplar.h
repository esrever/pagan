#pragma once

#include <vector>
#include <string>

#include <core/util/json_conversions.h>
#include "ecs_config.h"
#include "EntityComponents.h"

namespace pgn
{
	struct cExemplar
	{
		std::string				 mName;
		std::vector<std::string> mTags;
		cEntityComponents		 mComponents;
	};

	//---------------------------------------------
	template<>
	inline void to_json<cExemplar>(const cExemplar& zObj, JsonWriter& writer)
	{
		writer.StartObject();
		JsonWriter_AddMember("Name", zObj.mName, writer);
		JsonWriter_AddMember("Tags", zObj.mTags, writer);
		JsonWriter_AddMember("Data", zObj.mComponents, writer);
		writer.EndObject();
	}
}