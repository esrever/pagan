#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <oxygine-framework.h>
#include <core/container/Array2D.h>
#include <core/util/json_conversions.h>


namespace pgn
{
    namespace cmp
    {
		struct cLevelPosition;
        //! 
        struct cMapWindow
        {
		public:

			oxygine::Vector2 LevelToScreenCoords(const cLevelPosition& pos);
			//oxygine::spSprite& Tile(size_t col, size_t row) { return mTiles.at(col + row*mCellDims.x); }

		//private:
		//	DECL_JSON_PGN_FRIEND

			//std::vector<oxygine::spSprite> mTiles;
			glm::ivec2 mStartPos;
			glm::uvec2 mCellDims;
			glm::uvec2 mTileDims;
			oxygine::spClipRectActor mTileWin;

			cArray2D<oxygine::spSprite> mSprites;
        };
        
    }

	DECL_JSON_PGN(cmp::cMapWindow)
}