#include "Movement.h"

namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	void from_json<cMovement>( cMovement& zData, const rapidjson::Value& zRoot)
	{
		from_json( zData.mSpeed, zRoot["speed"]);
	}
}