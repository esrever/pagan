#include "TileLayout.h"

#include <ecs/ecs.h>


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cTileLayout>( cmp::cTileLayout& zData, const rapidjson::Value& zRoot)
	{
		/* TODO:
			Provide a map in some form, also provide default passable and default obstacles
			In export time, I bunch of uints which I can compress by paletting. This is true for layout only,
			and not for monsters and items. With paletting, I can get a few bits per tile, and I can use the binvox technique (or RLE)
			to compress: ( bitval - numTimes)
		*/
		glm::uvec2 dims;
		std::string dw, df,dd;
		from_json(dims, zRoot["dims"]);
		from_json(dw, zRoot["def_wall"]);
		from_json(df, zRoot["def_floor"]);
		from_json(dd, zRoot["def_door"]);

		auto& ecs = ECS;
		zData.mDefaultWall = ecs.mEntityMgr->GetEntityData().find( ecs.mEntityMgr->InstantiateExemplar(dw));
		zData.mDefaultFloor = ecs.mEntityMgr->GetEntityData().find(ecs.mEntityMgr->InstantiateExemplar(df));
		zData.mDefaultDoor = ecs.mEntityMgr->GetEntityData().find(ecs.mEntityMgr->InstantiateExemplar(dd));

		zData.mData.Resize(dims.x, dims.y);
		for (unsigned i = 0; i < dims.y;++i)
			for (unsigned j = 0; j < dims.x; ++j)
				zData.mData(j, i) = rand() & 1 ? zData.mDefaultWall : zData.mDefaultFloor;

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