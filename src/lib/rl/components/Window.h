#pragma once

#include <core/util/json_conversions.h>


namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cWindow
        {
            
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cWindow>( cmp::cWindow& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cWindow>( const cmp::cWindow& zData, JsonWriter& zRoot);
}