#pragma once

#include <bitset>
#include <sstream>

#include <core/util/json_conversions.h>

#include "ecs_config.h"
#include "Event.h"

namespace pgn 
{
	namespace evt
	{
		typedef cEvent<size_t(eBasicECS::ENTITY_CREATED), cEntity> cEntityCreated;
		typedef cEvent<size_t(eBasicECS::ENTITY_DESTROY), cEntity> cEntityDestroy;
		typedef cEvent<size_t(eBasicECS::ENTITY_TAGGED), cEntity, const std::string&> cEntityTagged;
		typedef cEvent<size_t(eBasicECS::ENTITY_UNTAG), cEntity, const std::string&> cEntityUntag;
		typedef cEvent<size_t(eBasicECS::TAG_REMOVE), const std::string&> cTagRemove;
	}

	//! Json conversions
	template<>
	void to_json<cEntity>(const cEntity& zEntity, JsonWriter& zRoot);

	template<>
	bool from_json<cEntity>(cEntity& zEntity, const rapidjson::Value& zRoot);
}

