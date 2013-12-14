#include "TileLayout.h"
#include "MapSprite.h"

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
		zData.mDefaultWall =  ecs.mEntityMgr->InstantiateExemplar(dw);
		zData.mDefaultFloor = ecs.mEntityMgr->InstantiateExemplar(df);
		zData.mDefaultDoor = ecs.mEntityMgr->InstantiateExemplar(dd);

		zData.mLayoutNode = new oxygine::Actor;

		zData.mData.Resize(dims.x, dims.y);
		for (unsigned i = 0; i < dims.y; ++i)
		{
			for (unsigned j = 0; j < dims.x; ++j)
			{
				// TODO: use visitors?
				auto ed = rand() & 1 ? ECS.mEntityMgr->CloneEntity(zData.mDefaultWall) : ECS.mEntityMgr->CloneEntity(zData.mDefaultFloor);
				std::shared_ptr< cComponent<pgn::cmp::cMapSprite>> sprite_ptr;
				ed->second.mComponents.GetComponent(sprite_ptr);

				//set positions
				sprite_ptr->mData.mSprite->setPosition(float(j), float(i));

				//Attach to layout node
				sprite_ptr->mData.mSprite->attachTo(zData.mLayoutNode);
				zData.mData(j, i) = ed;

				//TODO: add level position component!
			}
		}

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