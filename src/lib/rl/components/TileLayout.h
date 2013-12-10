#pragma once

#include <core/container/Array2D.h>
#include <core/util/json_conversions.h>

#include <ecs/EntityData.h>

#include <rl/util/visit.h>

namespace pgn
{
    namespace cmp
    {
        //! 
        struct cTileLayout
        {
			cArray2D<cEntityWithData> mData;
			
			cEntityWithData mDefaultWall;
			cEntityWithData mDefaultFloor;
			cEntityWithData mDefaultDoor;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cTileLayout>( cmp::cTileLayout& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cTileLayout>( const cmp::cTileLayout& zData, JsonWriter& zRoot);
}