#include "GameResources.h"

namespace pgn
{
	template<>
	bool from_json< cGameResources >(cGameResources & zData, const rapidjson::Value& zRoot)
	{
		from_json(zData.mTileLib, zRoot["Tile"]);
		from_json(zData.mFontLib, zRoot["Font"]);
		return true;
	}

	template<>
	void to_json< cGameResources >(const cGameResources & zData, JsonWriter& zRoot)
	{

	}
}