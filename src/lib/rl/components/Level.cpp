#include "Level.h"

#include <ecs/ecs.h>
#include "TileObstacle.h"


namespace pgn
{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cLevel>( cmp::cLevel& zData, const rapidjson::Value& zRoot)
	{
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

		zData.mLayoutData.Resize(dims.x, dims.y);
		for (unsigned i = 0; i < dims.y; ++i)
		{
			for (unsigned j = 0; j < dims.x; ++j)
			{
				zData.mLayoutData(j, i) = (rand() & 3) > 0;
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
		for (auto x : mDefaults)
		{
			auto obstacle_ptr = x->second.mComponents.GetComponent<pgn::cmp::cTileObstacle>();
			obstacle_ptr ? obstacles.push_back(obstacle_ptr->mData) : obstacles.push_back(pgn::cmp::cTileObstacle());
		}
		while (true)
		{
			int coord = rand();
			int y = int(coord / mLayoutData.Width()) % mLayoutData.Height();
			int x = coord % mLayoutData.Width();
			auto idx = mLayoutData(x, y);
			if (!obstacles.at(idx).mIsObstacle)
				return glm::ivec2(x, y);
		}
	}
}