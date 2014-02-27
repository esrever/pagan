#pragma once

#include <oxygine-framework.h>
#include <core/util/json_conversions.h>
#include <ecs/ecs_config.h>


namespace pgn
{
	enum class eMapRenderOrder : short
	{
		DungeonElementBG = 0,
		DungeonElementFG,
		Item,
		Creature,
		Effect,
		UI,
	};

    namespace cmp
    {
        //! 
        struct cMapSprite
        {
			cMapSprite();
			oxygine::spSprite mSprite;
			eMapRenderOrder	  mRenderPriority;

			void ReplaceFrame(const cmp::cMapSprite& s);
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