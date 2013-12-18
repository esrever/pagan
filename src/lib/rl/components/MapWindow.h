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
        //! Represents the map window, converts between level<->map coords and stores the BG sprites of the map
        struct cMapWindow
        {
		public:

			oxygine::Vector2 LevelToScreenCoords(const cLevelPosition& pos);

		//private:
		//	DECL_JSON_PGN_FRIEND

			//! Start position in level coordinates
			glm::ivec2 mStartPos;
			//! Number of tiles
			glm::uvec2 mCellDims;
			//! Pixel size of each tile
			glm::uvec2 mTileDims;
			//! The window actor, all tiles attach to this
			oxygine::spClipRectActor mTileWin;
			//! All sprites, cloned from the few entities of the level
			cArray2D<oxygine::spSprite> mSprites;
        };
        
    }

	DECL_JSON_PGN(cmp::cMapWindow)
}