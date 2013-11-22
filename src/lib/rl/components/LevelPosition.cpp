#include "LevelPosition.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cLevelPosition>( cmp::cLevelPosition& zData, const rapidjson::Value& zRoot)
	{
        from_json( zData.mPos, zRoot["Position"]);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cLevelPosition>( const cmp::cLevelPosition& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        JsonWriter_AddMember("Position", zData.mPos, zRoot);
		zRoot.EndObject();
	}
}