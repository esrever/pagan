#pragma once

#include <oxygine-framework.h>
#include <core/util/json_conversions.h>
#include <ecs/ecs_config.h>


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

	//-----------------------------------------------------------------------
	//! Clone the map sprite as it's gonna have a new position!
	template<>	inline cmp::cMapSprite Clone<cmp::cMapSprite>(const cmp::cMapSprite& val)
	{
		cmp::cMapSprite outp;
		outp.mSprite = val.mSprite->clone();
		return outp;
	}
}