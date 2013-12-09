#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <oxygine-framework.h>
#include <core/util/json_conversions.h>


namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cMapWindow : public oxygine::Actor
        {
		public:

			oxygine::spSprite& Tile(size_t col, size_t row) { return mTiles.at(col + row*mCellDims.x); }

		private:
			DECL_JSON_PGN_FRIEND

			std::vector<oxygine::spSprite> mTiles;
			glm::uvec2 mCellDims;
			glm::uvec2 mTileDims;
			
        };
        
    }

	DECL_JSON_PGN(cmp::cMapWindow)
}