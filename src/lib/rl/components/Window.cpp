#include "Window.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cWindow>( cmp::cWindow& zData, const rapidjson::Value& zRoot)
	{
        
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cWindow>( const cmp::cWindow& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        
		zRoot.EndObject();
	}
}