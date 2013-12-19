#include "Level.h"

#include <ecs/ecs.h>
#include "TileObstacle.h"
#include "LevelPosition.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cLevel>( cmp::cLevel& zData, const rapidjson::Value& zRoot)
	{
		glm::uvec2 dims;
		from_json(dims, zRoot["dims"]);

		// Read default values
		auto& ecs = ECS;
		const auto& defaults_bg = zRoot["defaults_bg"];
		for (auto it = defaults_bg.Begin(); it != defaults_bg.End(); ++it)
		{
			std::string name = it->GetString();
			auto ed = ecs.mEntityMgr->InstantiateExemplar(name);
			zData.mDefaultTilesBg.push_back(ed);
		}

		const auto& defaults = zRoot["defaults"];
		for (auto it = defaults.Begin(); it != defaults.End(); ++it)
		{
			std::string name = it->GetString();
			zData.mDefaultExemplars.push_back(name);
		}

		//Initialize bg tiles
		zData.mMapTileBg.Resize(dims.x, dims.y);
		for (unsigned i = 0; i < dims.y; ++i)
		{
			for (unsigned j = 0; j < dims.x; ++j)
			{
				zData.mMapTileBg(j, i) = (rand() & 3) > 0;
			}
		}

		// place some other entities
		zData.mMapTileRest.Resize(dims.x, dims.y);
		for (unsigned i = 0; i < dims.y; ++i)
		{
			for (unsigned j = 0; j < dims.x; ++j)
			{
				if (zData.mMapTileBg(j, i) > 0)
				{
					auto val = rand() % 50;
					for (size_t k = 0; k< zData.mDefaultExemplars.size(); ++k)
					{
						if (k == val)
						{
							auto ed = ecs.mEntityMgr->InstantiateExemplar(zData.mDefaultExemplars[k]);
							zData.mMapTileRest(j, i) = ed;
							cComponent<cmp::cLevelPosition> pos;
							pos.mData.mPos = glm::ivec2(j, i);
							ed->second.mComponents.AddComponent(std::make_shared < cComponent < cmp::cLevelPosition >> (pos));
						}
					}
					
				}
			}
		}
		

        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cLevel>( const cmp::cLevel& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        //JsonWriter_AddMember("IsObstacle", zData.mIsObstacle, zRoot);
		zRoot.EndObject();
	}

	//---------------------------------------------------------------------------------------
	glm::ivec2 cmp::cLevel::GetRandomStartPos()
	{
		std::vector< pgn::cmp::cTileObstacle> obstacles;
		for (auto x : mDefaultTilesBg)
		{
			auto obstacle_ptr = x->second.mComponents.GetComponent<pgn::cmp::cTileObstacle>();
			obstacle_ptr ? obstacles.push_back(obstacle_ptr->mData) : obstacles.push_back(pgn::cmp::cTileObstacle());
		}
		while (true)
		{
			int coord = rand();
			int y = int(coord / mMapTileBg.Width()) % mMapTileBg.Height();
			int x = coord % mMapTileBg.Width();
			auto idx = mMapTileBg(x, y);
			if (!obstacles.at(idx).mIsObstacle)
				return glm::ivec2(x, y);
		}
	}
}