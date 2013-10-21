#pragma once

#include <core/util/json_conversions.h>

namespace pgn
{
	//! Cost to move through a tile
	struct cTileMoveCost
	{
		float mMoveCost;
	};

	//-----------------------------------------------------------------------
	template<>
	void from_json< cTileMoveCost>( cTileMoveCost& zData, const rapidjson::Value& zRoot);
}