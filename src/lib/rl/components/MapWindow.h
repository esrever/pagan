#pragma once

#include <string>

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

		private:
			DECL_JSON_PGN_FRIEND

			oxygine::spColorRectSprite mBg;
			glm::uvec2 mCellDims;
			glm::uvec2 mTileDims;
			
        };
        
    }

	DECL_JSON_PGN(cmp::cMapWindow)
}