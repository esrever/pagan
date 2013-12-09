#include "MapWindow.h"
#include <rl/util/json_conversions.h>

using namespace oxygine;

namespace pgn
{

	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cMapWindow>( cmp::cMapWindow& zData, const rapidjson::Value& zRoot)
	{
		zData.mBg = new ColorRectSprite();

		from_json(zData.mBg, zRoot["bg"]);
		from_json(zData.mCellDims, zRoot["cell_dims"]);
		from_json(zData.mTileDims, zRoot["tile_dims"]);

		zData.mBg->setPosition(0, 0);
		auto size = zData.mCellDims * zData.mTileDims;
		zData.mBg->setSize(size.x, size.y);
		zData.mBg->attachTo(&zData);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cMapWindow>( const cmp::cMapWindow& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("bg", zData.mBg, zRoot);
		JsonWriter_AddMember("cell_dims", zData.mCellDims, zRoot);
		JsonWriter_AddMember("tile_dims", zData.mTileDims, zRoot);
		zRoot.EndObject();
	}
}