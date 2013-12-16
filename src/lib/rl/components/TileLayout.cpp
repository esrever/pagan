#include "TileLayout.h"

#include <ecs/ecs.h>

#include "MapSprite.h"
#include "LevelPosition.h"
#include "TileObstacle.h"

#include <rl/map/dungenrllib.h>


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

		//RL::InitRandomness();
		//cArray2D<size_t> mapvalues;
		//cDunGenRLLib_StdDungeon generator(20,false);
		//cDunGenRLLib_Caves generator;
		//cDunGenRLLib_SpaceShuttle generator;
		//cDunGenRLLib_AntNest generator;
		//generator.Generate(mapvalues, dims.x, dims.y);

		zData.mData.Resize(dims.x, dims.y);
		for (unsigned i = 0; i < dims.y; ++i)
		{
			for (unsigned j = 0; j < dims.x; ++j)
			{
				// TODO: use visitors?
				auto ed = rand() & 3 ? ECS.mEntityMgr->CloneEntity(zData.mDefaultFloor) : ECS.mEntityMgr->CloneEntity(zData.mDefaultWall);
				/*
				cEntityWithData ed;
				switch (mapvalues(j, i))
				{
					case RL::LevelElementWall:
						ed = ECS.mEntityMgr->CloneEntity(zData.mDefaultWall); break;
					case RL::LevelElementCorridor:
					case RL::LevelElementRoom:
					case RL::LevelElementGrass:
					case RL::LevelElementPlant:
					case RL::LevelElementWater:
					case RL::LevelElementDoorOpen:
					case RL::LevelElementDoorClose:
						ed = ECS.mEntityMgr->CloneEntity(zData.mDefaultFloor); break;
						//ed = ECS.mEntityMgr->CloneEntity(zData.mDefaultDoor); break;
					default:
						assert(false);
				}
				*/

				std::shared_ptr< cComponent<pgn::cmp::cMapSprite>> sprite_ptr;
				ed->second.mComponents.GetComponent(sprite_ptr);

				//set positions
				sprite_ptr->mData.mSprite->setPosition(float(j), float(i));

				//Attach to layout node
				sprite_ptr->mData.mSprite->attachTo(zData.mLayoutNode);
				zData.mData(j, i) = ed;

				//TODO: add level position component!
				std::shared_ptr< cComponent<pgn::cmp::cLevelPosition>> lvlpos_ptr = std::make_shared< cComponent<pgn::cmp::cLevelPosition>>();
				lvlpos_ptr->mData.mPos = glm::uvec2(j, i);
				ed->second.mComponents.AddComponent(lvlpos_ptr);
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

	//---------------------------------------------------------------------------------------
	glm::ivec2 GetRandomStartPos(const cmp::cTileLayout& layout)
	{
		while (true)
		{
			int coord = rand();
			int y = int(coord / layout.mData.Width()) % layout.mData.Height();
			int x = coord % layout.mData.Width();
			std::shared_ptr< cComponent<pgn::cmp::cTileObstacle>> obstacle_ptr;
			layout.mData(x, y)->second.mComponents.GetComponent(obstacle_ptr);
			if ( !(obstacle_ptr && obstacle_ptr->mData.mIsObstacle))
				return glm::ivec2(x, y);
		}
	}
}