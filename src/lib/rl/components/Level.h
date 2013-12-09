#pragma once

#include <core/util/json_conversions.h>


namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cLevel
        {
            
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cLevel>( cmp::cLevel& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cLevel>( const cmp::cLevel& zData, JsonWriter& zRoot);
}