#include "WorldTraits.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cWorldTraits>(cmp::cWorldTraits& zData, const rapidjson::Value& zRoot)
	{
        //from_json( zData.mSpeed, zRoot["Speed"]);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cWorldTraits>(const cmp::cWorldTraits& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        //JsonWriter_AddMember("Speed", zData.mSpeed, zRoot);
		zRoot.EndObject();
	}
}