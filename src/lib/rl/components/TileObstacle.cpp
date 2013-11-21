#include "TileObstacle.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cTileObstacle>( cTileObstacle& zData, const rapidjson::Value& zRoot)
	{
        from_json( zData.mIsObstacle, zRoot["IsObstacle"]);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cTileObstacle>( const cTileObstacle& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        JsonWriter_AddMember("IsObstacle", zData.mIsObstacle, zRoot);
		zRoot.EndObject();
	}
}