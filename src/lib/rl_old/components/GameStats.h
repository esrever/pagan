#pragma once

#include <map>
#include <core/util/json_conversions.h>
#include <ecs/Entity.h>


namespace pgn
{
    namespace cmp
    {
		//! 
        struct cGameStats
        {
			cGameStats();
			std::map<cEntity, size_t> mTotalTurns;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
	bool from_json< cmp::cGameStats>(cmp::cGameStats& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
	void to_json< cmp::cGameStats>(const cmp::cGameStats& zData, JsonWriter& zRoot);
}