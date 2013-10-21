#include "AsciiRep.h"

namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	void from_json<cAsciiRep>( cAsciiRep& zData, const rapidjson::Value& zRoot)
	{
		std::string symb;
		from_json( symb, zRoot["char"]);
		zData.mChar = symb[0];
		// TODO: read the rest, sanity checks too
		//from_json( zData.mVerbose, zRoot["color"]);
		//from_json( zData.mVerbose, zRoot["attribs"]);
	}
}