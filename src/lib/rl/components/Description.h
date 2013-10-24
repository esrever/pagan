#pragma once

#include <string>

#include <core/util/json_conversions.h>

namespace pgn
{
	//! 2-tier descriptions for components
	struct cDescription
	{
		std::string mShort;
		std::string mVerbose;
	};

	//-----------------------------------------------------------------------
	template<>
	bool from_json< cDescription>( cDescription& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< cDescription>( const cDescription& zData, rapidjson::Value& zRoot);
}