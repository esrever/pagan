#pragma once

#include <core/util/json_conversions.h>


namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cTileLayout
        {
            
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cTileLayout>( cmp::cTileLayout& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cTileLayout>( const cmp::cTileLayout& zData, JsonWriter& zRoot);
}