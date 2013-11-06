#include "TileMoveCost.h"

namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cTileMoveCost>( cTileMoveCost& zData, const rapidjson::Value& zRoot)
	{
		return from_json( zData.mMoveCost, zRoot["move_cost"]);
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cTileMoveCost>( const cTileMoveCost& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("move_cost", zData.mMoveCost, zRoot);
		zRoot.EndObject();
	}
}