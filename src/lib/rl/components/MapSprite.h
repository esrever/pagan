#pragma once

#include <oxygine-framework.h>
#include <core/util/json_conversions.h>


namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cMapSprite
        {
			oxygine::spSprite mSprite;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
	bool from_json< cmp::cMapSprite>(cmp::cMapSprite& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
	void to_json< cmp::cMapSprite>(const cmp::cMapSprite& zData, JsonWriter& zRoot);
}