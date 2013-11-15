#pragma once

#include <vector>
#include <string>

#include <core/util/json_conversions.h>

namespace pgn
{
	//! Specifies which loggers will this entity listen to
	struct cLoggerListener
	{
		std::vector<std::string> mLogTags;
	};

	//-----------------------------------------------------------------------
	template<>
	bool from_json< cLoggerListener>( cLoggerListener& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< cLoggerListener>( const cLoggerListener& zData, JsonWriter& zRoot);
}
