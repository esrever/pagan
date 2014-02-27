#include "Movement.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cMovement>( cmp::cMovement& zData, const rapidjson::Value& zRoot)
	{
        from_json( zData.mSpeed, zRoot["Speed"]);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cMovement>( const cmp::cMovement& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        JsonWriter_AddMember("Speed", zData.mSpeed, zRoot);
		zRoot.EndObject();
	}
}