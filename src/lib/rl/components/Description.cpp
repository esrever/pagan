#include "Description.h"

namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	void from_json<cDescription>( cDescription& zData, const rapidjson::Value& zRoot)
	{
		from_json( zData.mShort, zRoot["short"]);
		from_json( zData.mVerbose, zRoot["verbose"]);
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cDescription>( const cDescription& zData, rapidjson::Value& zRoot)
	{
		to_json( zData.mShort, zRoot["short"]);
		to_json( zData.mVerbose, zRoot["verbose"]);
	}
}