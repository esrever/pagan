#pragma once

#include <vector>
#include <string>

#include <core/util/json_conversions.h>

namespace pgn
{
	//! Game log
	struct cLogger
	{
		//std::vector<std::vector<chtype>> mLogData;
		std::vector<std::string> mLogData;
		size_t mMaxLines;
	};

	//-----------------------------------------------------------------------
	template<>
	bool from_json< cLogger>( cLogger& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< cLogger>( const cLogger& zData, JsonWriter& zRoot);

	//-----------------------------------------------------------------------
	void Log(cLogger& zLog, const std::string zMsg);
}