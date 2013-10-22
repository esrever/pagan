#include "AsciiRep.h"

namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	void from_json<cAsciiRep>( cAsciiRep& zData, const rapidjson::Value& zRoot)
	{
		from_json( zData.mChar, zRoot["char"]);
		// TODO: read the rest, sanity checks too
		//from_json( zData.mVerbose, zRoot["color"]);
		//from_json( zData.mVerbose, zRoot["attribs"]);
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cAsciiRep>( const cAsciiRep& zData, rapidjson::Value& zRoot)
	{
		to_json( zData.mChar, zRoot["char"]);
	}
}