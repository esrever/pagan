#pragma once

#include <core/util/json_conversions.h>


namespace pgn
{
    namespace cmp
    {
    
        //! Move traits
        struct cMovement
        {
            float mSpeed;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cMovement>( cmp::cMovement& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cMovement>( const cmp::cMovement& zData, JsonWriter& zRoot);
}