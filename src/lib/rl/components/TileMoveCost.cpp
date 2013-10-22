#include "TileMoveCost.h"

namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	void from_json<cTileMoveCost>( cTileMoveCost& zData, const rapidjson::Value& zRoot)
	{
		from_json( zData.mMoveCost, zRoot["move_cost"]);
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cTileMoveCost>( const cTileMoveCost& zData, rapidjson::Value& zRoot)
	{
		to_json( zData.mMoveCost, zRoot["move_cost"]);
	}
}