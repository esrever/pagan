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
}