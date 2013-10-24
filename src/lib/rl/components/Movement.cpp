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
	void to_json<cMovement>( const cMovement& zData, rapidjson::Value& zRoot)
	{
		to_json( zData.mSpeed, zRoot["speed"]);
	}
}