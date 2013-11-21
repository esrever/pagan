#pragma once

#include <core/util/json_conversions.h>


namespace pgn
{
	//! Can we move through this or not?
	struct cTileObstacle
	{
		bool mIsObstacle;
	};

	//-----------------------------------------------------------------------
	template<>
	bool from_json< cTileObstacle>( cTileObstacle& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< cTileObstacle>( const cTileObstacle& zData, JsonWriter& zRoot);
}