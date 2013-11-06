#pragma once

#include <curses.h>
#include <core/util/json_conversions.h>

namespace pgn
{
	//! ASCII render properties
	struct cAsciiRep
	{
		chtype mChar;
	};

	//-----------------------------------------------------------------------
	template<>
	bool from_json< cAsciiRep>( cAsciiRep& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< cAsciiRep>( const cAsciiRep& zData, JsonWriter& zRoot);
}