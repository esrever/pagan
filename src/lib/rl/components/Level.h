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
			//! Scene graph node that entities attach to 
			oxygine::spActor mActor;
			std::vector<cEntityWithData> mEntities;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cLevel>( cmp::cLevel& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cLevel>( const cmp::cLevel& zData, JsonWriter& zRoot);
}