#pragma once

#include <core/util/json_conversions.h>

namespace pgn
{
	//! 
	struct cMovement
	{
		float mSpeed;
	};

	//-----------------------------------------------------------------------
	template<>
	void from_json< cMovement>( cMovement& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< cMovement>( const cMovement& zData, rapidjson::Value& zRoot);
}