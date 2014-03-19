#include "events.h"

#include <core/util/string.h>
#include <rlut/components/components.h>
#include <rlut/fov/fov_rsc.h>


static const char * dirstrings_short[] = { "SW", "S", "SE", "W", "", "E", "NW", "N", "NE" };
static const char * dirstrings_long[] = { "southwest", "south", "southeast", "west", "nowhere", "east", "northwest", "north", "northeast" };

namespace pgn
{
	namespace rl
	{
		void RegisterActions(ecs::cECS& ecs)
		{
			for (int i = -1; i <= 1;++i)
				for (int j = -1; j <= 1; ++j)
				{
					int o = (i + 1) * 3 + j + 1;
					//cECS::cActionFunc func = std::bind(&evt::cPlayerMoveAdj::RunEvent, glm::ivec2(j, i));
					//ecs.RegisterAction(std::string("PlayerMove") + dirstrings_short[o], func);
				}
		}
	}

	template<>
	bool evt::cPlayerAppear::Run(const rl::cmp::cLocation& pos)
	{
		auto world = mainecs()->TagusToEntities("World")->second->second.Component<rl::cmp::cWorldData>();
		auto ped = mainecs()->TagusToEntities("Player")->second;
		if (evt::cAppear::Run(ped, pos))
		{
			auto loc = ped->second.Component<rl::cmp::cLocation>();
			// Set new current level
			mainecs()->Tagu("CurrentLevel", world->mLevelMap[loc->mLevelId]);
			return evt::cCalculateVisibility::Run(ped);
		}
		else
			return false;
	}

	template<>
	bool evt::cAppear::Run(ecs::cEntityWithData ed, const rl::cmp::cLocation& pos)
	{
		// set location
		auto loc = ed->second.Component<rl::cmp::cLocation>();
		*loc = pos;

		// set the actor in the level
		auto world = mainecs()->TagusToEntities("World")->second->second.Component<rl::cmp::cWorldData>();
		auto lvl = world->mLevelMap[loc->mLevelId]->second.Component<rl::cmp::cLevelData>();
		lvl->mLayout.SetActor(ed);
		return true;
	}

	template<>
	bool evt::cCreateLevel::Run(ecs::cEntityWithData ed)
	{
		// TODO: flesh out. Currently store the id -> level association
		auto level = ed->second.Component<rl::cmp::cLevelData>();
		auto world = mainecs()->TagusToEntities("World");
		world->second->second.Component<rl::cmp::cWorldData>()->mLevelMap[level->mId] = ed;
		return true;
	}

}