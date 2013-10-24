#include "AsciiRep.h"

namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	void from_json<cAsciiRep>( cAsciiRep& zData, const rapidjson::Value& zRoot)
	{
		char c;
		from_json( c, zRoot["char"]);
		
		/*
			Parse:
				char
				attr
				col_fg
				col_bg
			Create a color object (static), and fetch the color index, and set it
		*/
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cAsciiRep>( const cAsciiRep& zData, rapidjson::Value& zRoot)
	{
		to_json( zData.mChar, zRoot["char"]);
	}
}