#include "LevelWindow.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cLevelWindow>( cmp::cLevelWindow& zData, const rapidjson::Value& zRoot)
	{
        
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cLevelWindow>( const cmp::cLevelWindow& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        
		zRoot.EndObject();
	}
}