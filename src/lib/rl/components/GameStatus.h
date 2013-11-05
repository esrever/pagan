#pragma once

#include <vector>
#include <string>

#include <core/util/json_conversions.h>

namespace pgn
{
	//! Game Status
	struct cGameStatus
	{
		
	};

	//-----------------------------------------------------------------------
	template<>
	bool from_json< cGameStatus>( cGameStatus& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< cGameStatus>( const cGameStatus& zData, rapidjson::Value& zRoot);
}