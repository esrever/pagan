#pragma once

#include "Logger.h"

namespace pgn
{
	//-----------------------------------------------------------------------
	template<>
	bool from_json< cLogger>( cLogger& zData, const rapidjson::Value& zRoot)
	{
		from_json(zData.mMaxLines,"MaxLines");
		return true;
	}

	//-----------------------------------------------------------------------
	template<>
	void to_json< cLogger>( const cLogger& zData, JsonWriter& zRoot){}

	void Log(cLogger& zLog, const std::string zMsg)
	{

	}
}