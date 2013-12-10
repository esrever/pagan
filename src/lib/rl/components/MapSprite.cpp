#include "MapSprite.h"

#include <rl/GameApp.h>


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cMapSprite>(cmp::cMapSprite& zData, const rapidjson::Value& zRoot)
	{
		std::string tilename;
		from_json(tilename, zRoot);
		// We clone it as we don't want to be using the one from the lib
		zData.mSprite = GAME.GetResources().mTileLib.GetSprite(tilename)->clone();
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cMapSprite>(const cmp::cMapSprite& zData, JsonWriter& zRoot)
	{
		to_json(zData.mSprite ? zData.mSprite->getName() : "null", zRoot);
	}
}