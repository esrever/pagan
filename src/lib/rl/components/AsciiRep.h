#pragma once

#include <core/util/json_conversions.h>

namespace pgn
{
	//! ASCII render properties
	struct cAsciiRep
	{
		char mChar;
		unsigned mColor;
		unsigned mAttribs;
	};

	//-----------------------------------------------------------------------
	template<>
	void from_json< cAsciiRep>( cAsciiRep& zData, const rapidjson::Value& zRoot);
}