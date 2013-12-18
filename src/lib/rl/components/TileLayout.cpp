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
		from_json(dims, zRoot["dims"]);

		auto& ecs = ECS;
		const auto& defaults = zRoot["defaults"];
		for (auto it = defaults.Begin(); it != defaults.End(); ++it)
		{
			std::string name = it->GetString();
			auto ed = ecs.mEntityMgr->InstantiateExemplar(name);
			zData.mDefaults.push_back(ed);
		}

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
				//auto ed = rand() & 3 ? ECS.mEntityMgr->CloneEntity(zData.mDefaultFloor) : ECS.mEntityMgr->CloneEntity(zData.mDefaultWall);
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
				zData.mData(j, i) = (rand() & 3) > 0;
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
		std::vector< pgn::cmp::cTileObstacle> obstacles;
		for (auto x : layout.mDefaults)
		{
			auto obstacle_ptr = x->second.mComponents.GetComponent<pgn::cmp::cTileObstacle>();
			obstacles.push_back(obstacle_ptr->mData);
		}
		while (true)
		{
			int coord = rand();
			int y = int(coord / layout.mData.Width()) % layout.mData.Height();
			int x = coord % layout.mData.Width();
			auto idx = layout.mData(x, y);
			if ( ! obstacles.at(idx).mIsObstacle)
				return glm::ivec2(x, y);
		}
	}
}