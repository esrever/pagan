#pragma once

#include <core/util/json_conversions.h>

namespace pgn
{
	//! Traversable (or not) space
	struct cTileObstacle
	{
		bool mIsObstacle;
	};

	//-----------------------------------------------------------------------
	template<>
	bool from_json< cTileObstacle>( cTileObstacle& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< cTileObstacle>( const cTileObstacle& zData, rapidjson::Value& zRoot);
}