#pragma once

#include <core/container/Array2D.h>
#include <core/util/json_conversions.h>

#include <ecs/Entity.h>

namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cTileLayout
        {
			cArray2D<cEntity> mData;
			cEntity mDefaultWall;
			cEntity mDefaultFloor;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cTileLayout>( cmp::cTileLayout& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cTileLayout>( const cmp::cTileLayout& zData, JsonWriter& zRoot);
}