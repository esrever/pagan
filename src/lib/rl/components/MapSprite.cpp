#include "MapSprite.h"

#include <rl/GameApp.h>


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cMapSprite>(cmp::cMapSprite& zData, const rapidjson::Value& zRoot)
	{
		std::string tilename;
        //from_json( tilename, zRoot["tile"]);
		from_json(tilename, zRoot);
		zData.mSprite = GAME.GetResources().mTileLib.GetSprite(tilename)->clone();
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cMapSprite>(const cmp::cMapSprite& zData, JsonWriter& zRoot)
	{
		/*
		zRoot.StartObject();
		JsonWriter_AddMember("tile", zData.mSprite ? zData.mSprite->getName(): "null", zRoot);
		zRoot.EndObject();
		*/
		to_json(zData.mSprite ? zData.mSprite->getName() : "null", zRoot);
	}
}