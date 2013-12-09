#include "MapWindow.h"
#include <rl/util/json_conversions.h>

using namespace oxygine;

namespace pgn
{

	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cMapWindow>( cmp::cMapWindow& zData, const rapidjson::Value& zRoot)
	{
		from_json(zData.mCellDims, zRoot["cell_dims"]);
		from_json(zData.mTileDims, zRoot["tile_dims"]);

		size_t tilenum = zData.mCellDims.x*zData.mCellDims.y;
		zData.mTiles.resize(tilenum);
		for (unsigned i = 0; i < zData.mCellDims.y;++i)
			for (unsigned j = 0; j < zData.mCellDims.x; ++j)
			{
				auto o = i*zData.mCellDims.x + j;
				auto& tile = zData.mTiles[o];
				tile = new Sprite;
				tile->setSize(float(zData.mTileDims.x), float(zData.mTileDims.y));
				tile->setPosition(float(j*zData.mTileDims.x), float(i*zData.mTileDims.y));
				tile->attachTo(&zData);
			}
		oxygine::getRoot()->addChild(&zData); // TODO: on game state change
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cMapWindow>( const cmp::cMapWindow& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("cell_dims", zData.mCellDims, zRoot);
		JsonWriter_AddMember("tile_dims", zData.mTileDims, zRoot);
		zRoot.EndObject();
	}
}