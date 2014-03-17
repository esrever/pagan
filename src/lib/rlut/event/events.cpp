#include "events.h"

#include <rlut/components/components.h>


static const char * dirstrings_short[] = { "SW", "S", "SE", "W", "", "E", "NW", "N", "NE" };
static const char * dirstrings_long[] = { "southwest", "south", "southeast", "west", "", "east", "northwest", "north", "northeast" };

namespace pgn
{
	namespace rl
	{
		void RegisterActions(cECS& ecs)
		{
			for (int i = -1; i <= 1;++i)
				for (int j = -1; j <= 1; ++j)
				{
					int o = (i + 1) * 3 + j + 1;
					cECS::cActionFunc func = std::bind(&evt::cPlayerMoveAdj::RunEvent, glm::ivec2(j, i));
					ecs.RegisterAction(std::string("PlayerMove") + dirstrings_short[o], func);
				}
		}
	}

	template<>
	bool evt::cPlayerMoveAdj::Run(const glm::ivec2& dir)
	{
		auto ped = mainecs()->TagusToEntities("Player")->second;
		return evt::cMoveAdj::Run(ped, dir);
	}

	template<>
	bool evt::cMoveAdj::Run(cECS::cEntityWithData ed, const glm::ivec2& dir)
	{
		auto ptr = ed->second.Component<rl::cmp::cLocation>();

		// TODO: level updates position properly via events
		auto lvl = mainecs()->TagusToEntities("CurrentLevel")->second->second.Component<rl::cmp::cLevelData>();


		auto newpos = ptr->mPos + dir;
		// TODO: implement map checks with movecost map!
		if (!lvl->mLayout.BgEntities().InRange(newpos))
			return false;
		auto movecost = lvl->mLayout.BgEntities()(newpos.x, newpos.y)->second.Component<rl::cmp::cMoveCost>();
		if (movecost->mMoveCost < std::numeric_limits<float>::max())
		{
			lvl->mLayout.RemoveActor(ed);
			ptr->mPos = newpos;
			lvl->mLayout.AddActor(ed);
			return true;
		}
		return false;
	}
}