#pragma once

#include <core/util/json_conversions.h>


namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cOutStream
        {
            
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cOutStream>( cmp::cOutStream& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cOutStream>( const cmp::cOutStream& zData, JsonWriter& zRoot);
}