#pragma once

#include <core/util/json_conversions.h>
#include <glm/glm.hpp>

namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cLevelPosition
        {
            glm::uvec2 mPos;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cLevelPosition>( cmp::cLevelPosition& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cLevelPosition>( const cmp::cLevelPosition& zData, JsonWriter& zRoot);
}