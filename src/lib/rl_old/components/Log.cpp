#include "Log.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cLog>( cmp::cLog& zData, const rapidjson::Value& zRoot)
	{
		from_json(zData.mMaxLineNum, zRoot["MaxLineNum"]);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cLog>( const cmp::cLog& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("MaxLineNum", zData.mMaxLineNum, zRoot);
		zRoot.EndObject();
	}
}