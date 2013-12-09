#include "Level.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cLevel>( cmp::cLevel& zData, const rapidjson::Value& zRoot)
	{
       // from_json( zData.mIsObstacle, zRoot["IsObstacle"]);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cLevel>( const cmp::cLevel& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        //JsonWriter_AddMember("IsObstacle", zData.mIsObstacle, zRoot);
		zRoot.EndObject();
	}
}