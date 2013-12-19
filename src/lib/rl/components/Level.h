#pragma once

#include <vector>
#include <oxygine-framework.h>
#include <core/container/Array2D.h>
#include <core/util/json_conversions.h>
#include <ecs/EntityData.h>
#include <rl/util/visit.h>


namespace pgn
{
	namespace cmp
	{

		//! 
		struct cLevel
		{
			cLevel() :mLevelNode(new oxygine::Actor){}

			// Members

			//! Scene graph node that entities attach to 
			oxygine::spActor mLevelNode;

			//! Entities in this level
			std::set<cEntityWithData> mEntities;
			
			//! TODO: add the spatial level graph here!

			//! Array of indices to the defaults of the layout
			cArray2D<uint8_t> mLayoutData;

			//! Default entities of the level
			std::vector<cEntityWithData> mDefaults;

			//! General functions
			cEntityWithData LookupEntity(int x, int y) { return mDefaults.at(mLayoutData(x, y)); }
			glm::ivec2 GetRandomStartPos();
		};

	}

	//-----------------------------------------------------------------------
	template<>
	bool from_json< cmp::cLevel>(cmp::cLevel& zData, const rapidjson::Value& zRoot);
	//-----------------------------------------------------------------------
	template<>
	void to_json< cmp::cLevel>(const cmp::cLevel& zData, JsonWriter& zRoot);
}