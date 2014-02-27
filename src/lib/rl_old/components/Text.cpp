#include "Text.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cText>( cmp::cText& zData, const rapidjson::Value& zRoot)
	{
        from_json( zData.mText, zRoot["text"]);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cText>( const cmp::cText& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        JsonWriter_AddMember("text", zData.mText, zRoot);
		zRoot.EndObject();
	}
}