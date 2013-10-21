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
	void from_json< cTileObstacle>( cTileObstacle& zData, const rapidjson::Value& zRoot);
}