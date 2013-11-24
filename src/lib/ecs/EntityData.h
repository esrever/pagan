#pragma once

#include <string>

#include "EntityComponents.h"

namespace pgn
{
	struct cEntityData
	{
		std::string		  mName;
		cEntityComponents mComponents;
	};

	typedef std::map<cEntity, cEntityData>::const_iterator cEntityWithData;

	namespace evt
	{
		typedef cEvent<size_t(eBasicECS::COMPONENT_ADDED), cEntityWithData, unsigned short> cComponentAdded;
		typedef cEvent<size_t(eBasicECS::COMPONENTS_ADDED), cEntityWithData> cComponentsAdded;
		typedef cEvent<size_t(eBasicECS::COMPONENT_REMOVE), cEntityWithData, unsigned short> cComponentRemove;
	}

	//---------------------------------------------
	template<>
	inline void to_json<cEntityData>(const cEntityData& zObj, JsonWriter& writer)
	{
		writer.StartObject();
		JsonWriter_AddMember("Name", zObj.mName, writer);
		JsonWriter_AddMember("Components", zObj.mComponents, writer);
		writer.EndObject();
	}
}