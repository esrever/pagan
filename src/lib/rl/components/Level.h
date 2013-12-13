#pragma once

#include <vector>
#include <oxygine-framework.h>
#include <core/util/json_conversions.h>
#include <ecs/EntityData.h>


namespace pgn
{
	namespace cmp
	{

		//! 
		struct cLevel
		{
			cLevel() :mLevelNode(new oxygine::Actor){}
			//! Scene graph node that entities attach to 
			oxygine::spActor mLevelNode;
			std::vector<cEntityWithData> mEntities;
			//! TODO: add the spatial level graph here!
		};

	}

	//-----------------------------------------------------------------------
	template<>
	bool from_json< cmp::cLevel>(cmp::cLevel& zData, const rapidjson::Value& zRoot);
	//-----------------------------------------------------------------------
	template<>
	void to_json< cmp::cLevel>(const cmp::cLevel& zData, JsonWriter& zRoot);
}