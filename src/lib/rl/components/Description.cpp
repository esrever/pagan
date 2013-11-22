#include "Description.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cDescription>( cmp::cDescription& zData, const rapidjson::Value& zRoot)
	{
        from_json( zData.mVerbose, zRoot["Verbose"]);
		from_json( zData.mShort, zRoot["Short"]);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cDescription>( const cmp::cDescription& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        JsonWriter_AddMember("Verbose", zData.mVerbose, zRoot);
		JsonWriter_AddMember("Short", zData.mShort, zRoot);
		zRoot.EndObject();
	}
}