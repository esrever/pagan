#pragma once

#include "GameStatus.h"

namespace pgn
{
	//-----------------------------------------------------------------------
	template<>
	bool from_json< cGameStatus>( cGameStatus& zData, const rapidjson::Value& zRoot)
	{
		return true;
	}

	//-----------------------------------------------------------------------
	template<>
	void to_json< cGameStatus>( const cGameStatus& zData, rapidjson::Value& zRoot){}
}