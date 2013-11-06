#include "Description.h"

namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cDescription>( cDescription& zData, const rapidjson::Value& zRoot)
	{
		return from_json( zData.mShort, zRoot["short"]) &&
			   from_json( zData.mVerbose, zRoot["verbose"]);
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cDescription>( const cDescription& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("short", zData.mShort,zRoot);
		JsonWriter_AddMember("verbose", zData.mVerbose, zRoot);
		zRoot.EndObject();
	}
}