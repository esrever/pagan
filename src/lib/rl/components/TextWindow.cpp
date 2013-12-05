#include "TextWindow.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cTextWindow>( cmp::cTextWindow& zData, const rapidjson::Value& zRoot)
	{
        
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cTextWindow>( const cmp::cTextWindow& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        
		zRoot.EndObject();
	}
}