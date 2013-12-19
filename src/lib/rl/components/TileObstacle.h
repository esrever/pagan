#pragma once

#include <core/util/json_conversions.h>


namespace pgn
{
    namespace cmp
    {
    
        //! Can we move through this or not?
        struct cTileObstacle
        {
			cTileObstacle() :mIsObstacle(false){}
            bool mIsObstacle;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cTileObstacle>( cmp::cTileObstacle& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cTileObstacle>( const cmp::cTileObstacle& zData, JsonWriter& zRoot);
}