#pragma once

#include <bitset>
#include <sstream>

#include <core/util/json_conversions.h>

#include "ecs_config.h"
#include "Event.h"

namespace pgn 
{
	typedef cEvent<size_t(eBasicECSEvent::ENTITY_CREATED), cEntity> cEntityCreatedEvent;
	typedef cEvent<size_t(eBasicECSEvent::ENTITY_DESTROY), cEntity> cEntityDestroyEvent;
	typedef cEvent<size_t(eBasicECSEvent::ENTITY_TAGGED), cEntity, const std::string&> cEntityTaggedEvent;
	typedef cEvent<size_t(eBasicECSEvent::ENTITY_UNTAG), cEntity, const std::string&> cEntityUntagEvent;
	typedef cEvent<size_t(eBasicECSEvent::TAG_REMOVE), const std::string&> cTagRemoveEvent;

	//! Json conversions
	template<>
	void to_json<cEntity>(const cEntity& zEntity, JsonWriter& zRoot);

	template<>
	bool from_json<cEntity>(cEntity& zEntity, const rapidjson::Value& zRoot);
}

