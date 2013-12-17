#include "FormatString.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cFormatString>( cmp::cFormatString& zData, const rapidjson::Value& zRoot)
	{
        from_json( zData.mText, zRoot["text"]);
		from_json(zData.mDict, zRoot["dict"]);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cFormatString>( const cmp::cFormatString& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        JsonWriter_AddMember("dict", zData.mDict, zRoot);
		zRoot.EndObject();
	}
}