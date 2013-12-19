#include "Door.h"

#include <rl/GameApp.h>


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cDoor>(cmp::cDoor& zData, const rapidjson::Value& zRoot)
	{
		const auto& states = zRoot["sprites"];
		int i = 0;
		for (auto it = states.Begin(); it != states.End(); ++it)
			from_json(zData.mSprites[i++], *it);
		zData.mSprites[0].mRenderPriority = eMapRenderOrder::DungeonElementFG;
		zData.mSprites[1].mRenderPriority = eMapRenderOrder::DungeonElementFG;
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