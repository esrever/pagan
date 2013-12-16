#pragma once

#include <vector>
#include <string>

#include "EntityComponents.h"

namespace pgn
{
	struct cEntityData
	{
		std::string					mName;
		cEntityComponents			mComponents;
		std::set<std::string>		mTags;
	};

	//typedef std::map<cEntity, cEntityData>::value_type	   cEntityWithData;
	typedef std::map<cEntity, cEntityData>::iterator cEntityWithData;
	typedef std::map<cEntity, cEntityData>::const_iterator cEntityWithDataConst;

	namespace evt
	{
		typedef cEvent<size_t(eBasicECS::ENTITY_CREATED), cEntityWithData> cEntityCreated;
		typedef cEvent<size_t(eBasicECS::ENTITY_DESTROY), cEntityWithData> cEntityDestroy;
		typedef cEvent<size_t(eBasicECS::COMPONENT_ADDED), cEntityWithData, unsigned short> cComponentAdded;
		typedef cEvent<size_t(eBasicECS::COMPONENTS_ADDED), cEntityWithData> cComponentsAdded;
		typedef cEvent<size_t(eBasicECS::COMPONENT_REMOVE), cEntityWithData, unsigned short> cComponentRemove;
		typedef cEvent<size_t(eBasicECS::ENTITY_TAGGED), cEntityWithData, const std::string&> cEntityTagged;
		typedef cEvent<size_t(eBasicECS::ENTITY_UNTAG), cEntityWithData, const std::string&> cEntityUntag;
	}

	//--------------------------------------------------------------
	typedef cAction<size_t(evt::eBasicECS::ENTITY_CREATED), cEntityWithData> cActionEntityCreated;
	// TODO: typedef the rest

	//---------------------------------------------
	template<>
	inline void to_json<cEntityData>(const cEntityData& zObj, JsonWriter& writer)
	{
		writer.StartObject();
		JsonWriter_AddMember("Name", zObj.mName, writer);
		JsonWriter_AddMember("Components", zObj.mComponents, writer);
		JsonWriter_AddMember("Tags", zObj.mTags, writer);
		writer.EndObject();
	}
}

/*
inline bool operator < (const pgn::cEntityWithData& e0, const pgn::cEntityWithData& e1)
{
	return e0->first < e1->first;
}

inline bool operator < (const pgn::cEntityWithDataConst& e0, const pgn::cEntityWithDataConst& e1)
{
	return e0->first < e1->first;
}
*/