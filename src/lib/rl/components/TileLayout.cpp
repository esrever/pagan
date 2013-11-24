#include "TileLayout.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cTileLayout>( cmp::cTileLayout& zData, const rapidjson::Value& zRoot)
	{
		/*
			Provide a map in some form, also provide default passable and default obstacles
			In export time, I bunch of uints which I can compress by paletting. This is true for layout only,
			and not for monsters and items. With paletting, I can get a few bits per tile, and I can use the binvox technique (or RLE)
			to compress: ( bitval - numTimes)
		*/
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cTileLayout>( const cmp::cTileLayout& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        
		zRoot.EndObject();
	}
}