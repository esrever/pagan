#include "TileObstacle.h"

namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cTileObstacle>( cTileObstacle& zData, const rapidjson::Value& zRoot)
	{
		return from_json( zData.mIsObstacle, zRoot["is_obstacle"]);
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cTileObstacle>( const cTileObstacle& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("is_obstacle", zData.mIsObstacle, zRoot);
		zRoot.EndObject();
	}
}