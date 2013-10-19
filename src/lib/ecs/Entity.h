#pragma once

#include <bitset>
#include <sstream>

#include <core/util/conversions.h>
#include <core/util/json_conversions.h>

#include "ecs_config.h"
#include "Event.h"

namespace pgn 
{

	DECL_EVENT1(EntityCreated, cEntity);
	DECL_EVENT1(DestroyEntity, cEntity);

	//! entity to json
	template<>
	void to_json<cEntity>(const cEntity& zEntity, rapidjson::Value& zRoot);
	//! entity from json
	template<>
	void from_json<cEntity>(cEntity& zEntity, const rapidjson::Value& zRoot);
}

