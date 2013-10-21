#include "TileObstacle.h"

namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	void from_json<cTileObstacle>( cTileObstacle& zData, const rapidjson::Value& zRoot)
	{
		from_json( zData.mIsObstacle, zRoot["is_obstacle"]);
	}
}