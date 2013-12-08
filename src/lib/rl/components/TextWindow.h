#pragma once

#include <string>

#include <oxygine-framework.h>
#include <core/util/json_conversions.h>


namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cTextWindow : public oxygine::Actor
        {
		public:
			void SetText(const std::string& zText);

		private:
			DECL_JSON_PGN_FRIEND

			oxygine::spColorRectSprite mBg;
			oxygine::spTextActor mText;
			oxygine::TextStyle mStyle;
			oxygine::Vector2 mPos;
			oxygine::Vector2 mSize;
			
        };
        
    }

	DECL_JSON_PGN(cmp::cTextWindow)
}