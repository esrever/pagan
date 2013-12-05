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
			void Init();
			void SetText(const std::string& zText);

		private:
			template<class T>
			friend bool from_json(cmp::cTextWindow& zData, const rapidjson::Value& zRoot);
			template<class T>
			friend void to_json< cmp::cTextWindow>(const cmp::cTextWindow& zData, JsonWriter& zRoot);
		private:

			oxygine::spColorRectSprite mBg;
			oxygine::spTextActor mText;
			oxygine::TextStyle mStyle;
			oxygine::Vector2 mStart;
			oxygine::Vector2 mSize;
			
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cTextWindow>( cmp::cTextWindow& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cTextWindow>( const cmp::cTextWindow& zData, JsonWriter& zRoot);
}