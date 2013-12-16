#pragma once

#include <core/util/json_conversions.h>
#include <glm/glm.hpp>

#include <ecs/EntityData.h>

namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cLevelPosition
        {
            glm::ivec2 mPos;
			cEntityWithData mLevel;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cLevelPosition>( cmp::cLevelPosition& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cLevelPosition>( const cmp::cLevelPosition& zData, JsonWriter& zRoot);
}