#include "Door.h"

#include <rl/GameApp.h>


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cDoor>(cmp::cDoor& zData, const rapidjson::Value& zRoot)
	{
		std::string tile_open;
		std::string tile_closed;
		from_json(zData.mSprites[0], zRoot["tile_open"]);
		from_json(zData.mSprites[1], zRoot["tile_closed"]);
		from_json(zData.mIsClosed, zRoot["is_closed"]);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cDoor>(const cmp::cDoor& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("tile_open", zData.mSprites[0], zRoot);
		JsonWriter_AddMember("tile_closed", zData.mSprites[1], zRoot);
		JsonWriter_AddMember("is_closed", zData.mIsClosed, zRoot);
		zRoot.EndObject();
	}
}