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
	DECL_EVENT2(EntityTagged,cEntity,std::string);
	DECL_EVENT2(EntityUntag,cEntity,std::string);
	DECL_EVENT1(Untag,std::string);

	//! Json conversions
	template<>
	void to_json<cEntity>(const cEntity& zEntity, rapidjson::Value& zRoot);

	template<>
	bool from_json<cEntity>(cEntity& zEntity, const rapidjson::Value& zRoot);
}

