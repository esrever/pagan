#include "GameStats.h"


namespace pgn
{
	cmp::cGameStats::cGameStats() {}
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cGameStats>( cmp::cGameStats& zData, const rapidjson::Value& zRoot)
	{
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cGameStats>( const cmp::cGameStats& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		zRoot.EndObject();
	}
}