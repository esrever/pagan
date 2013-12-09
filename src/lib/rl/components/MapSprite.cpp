#include "MapSprite.h"

#include <rl/GameApp.h>


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cMapSprite>(cmp::cMapSprite& zData, const rapidjson::Value& zRoot)
	{
		std::string tilename;
        from_json( tilename, zRoot["tile"]);
		GAME.GetResources().mTileLib.GetSprite(tilename);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cMapSprite>(const cmp::cMapSprite& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        JsonWriter_AddMember("tile", zData.mSprite->getName(), zRoot);
		zRoot.EndObject();
	}
}