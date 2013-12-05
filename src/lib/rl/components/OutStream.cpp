#include "OutStream.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cOutStream>( cmp::cOutStream& zData, const rapidjson::Value& zRoot)
	{
        
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cOutStream>( const cmp::cOutStream& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        
		zRoot.EndObject();
	}
}