#include "Movement.h"

namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cMovement>( cMovement& zData, const rapidjson::Value& zRoot)
	{
		return from_json( zData.mSpeed, zRoot["speed"]);
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cMovement>( const cMovement& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("speed", zData.mSpeed, zRoot);
		zRoot.EndObject();
	}
}