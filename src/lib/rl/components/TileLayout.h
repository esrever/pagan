#pragma once

#include <cstdint>
#include <oxygine-framework.h>
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
			cArray2D<uint8_t> mData;
			std::vector<cEntityWithData> mDefaults;

			//! General functions
			cEntityWithData LookupEntity(int x, int y) { return mDefaults.at(mData(x, y)); }
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cTileLayout>( cmp::cTileLayout& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cTileLayout>( const cmp::cTileLayout& zData, JsonWriter& zRoot);

	//-----------------------------------------------------------------------
	// Misc functions
	glm::ivec2 GetRandomStartPos(const cmp::cTileLayout& layout);
}