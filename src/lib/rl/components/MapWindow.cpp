#include "MapWindow.h"
#include "LevelPosition.h"
#include <rl/util/json_conversions.h>

using namespace oxygine;

namespace pgn
{
	//----------------------------------------------------------------------------------
	oxygine::Vector2 cmp::cMapWindow::LevelToScreenCoords(const cmp::cLevelPosition& pos)
	{
		auto vpos = (mStartPos + pos.mPos)*glm::ivec2(mTileDims);
		vpos.y = (mCellDims.y-1)*mTileDims.y - vpos.y;
		return oxygine::Vector2(float(vpos.x), float(vpos.y));
	}

	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cMapWindow>( cmp::cMapWindow& zData, const rapidjson::Value& zRoot)
	{
		zData.mStartPos = glm::uvec2(0, 0);

		from_json(zData.mCellDims, zRoot["cell_dims"]);
		from_json(zData.mTileDims, zRoot["tile_dims"]);

		auto mapsize = zData.mCellDims * zData.mTileDims;

		zData.mTileWin = new oxygine::ClipRectActor;
		zData.mTileWin->setEnableClipping(true);
		zData.mTileWin->setPosition(0, 0);
		zData.mTileWin->setSize(float(mapsize.x), float(mapsize.y));
		oxygine::getRoot()->addChild(zData.mTileWin); // TODO: on game state change
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