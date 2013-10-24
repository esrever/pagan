#pragma once

#include <bitset>
#include <sstream>

#include <core/util/json_conversions.h>

#include "ecs_config.h"
#include "Event.h"

namespace pgn 
{

	DECL_EVENT1(EntityCreated, cEntity);
	DECL_EVENT1(DestroyEntity, cEntity);

	//! Json conversions
	template<>
	void to_json<cEntity>(const cEntity& zEntity, rapidjson::Value& zRoot);

	template<>
	bool from_json<cEntity>(cEntity& zEntity, const rapidjson::Value& zRoot);
}

