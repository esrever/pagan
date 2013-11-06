#pragma once

#include <vector>
#include <string>

#include <core/util/json_conversions.h>

namespace pgn
{
	//! Game log
	struct cGameLog
	{
		//std::vector<std::vector<chtype>> mLogData;
		std::vector<std::string> mLogData;
		size_t mMaxLines;
	};

	//-----------------------------------------------------------------------
	template<>
	bool from_json< cGameLog>( cGameLog& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< cGameLog>( const cGameLog& zData, JsonWriter& zRoot);
}