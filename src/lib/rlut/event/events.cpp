#include "events.h"

#include <rlut/components/components.h>
#include <rlut/fov/fov_rsc.h>


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
		if (evt::cMoveAdj::Run(ped, dir))
			return evt::cCalculateVisibility::Run(ped);
		else
			return false;
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
	bool evt::cMoveAdj::Run(cECS::cEntityWithData ed, const glm::ivec2& dir)
	{
		// get some components
		auto loc = ed->second.Component<rl::cmp::cLocation>();
		auto world = mainecs()->TagusToEntities("World")->second->second.Component<rl::cmp::cWorldData>();
		auto lvl = world->mLevelMap[loc->mLevelId]->second.Component<rl::cmp::cLevelData>();
		
		// calc new position
		auto newpos = loc->mPos + dir;

		// Map checks with obstacle map!
		if (!lvl->mLayout.BgEntities().InRange(newpos))
			return false;
		if (!lvl->mLayout.Obstacles()(newpos))
		{
			// no collisions; set new position and update map
			loc->mPos = newpos;
			lvl->mLayout.SetActor(ed);
			return true;
		}
		return false;
	}

	template<>
	bool evt::cAppear::Run(cECS::cEntityWithData ed, const rl::cmp::cLocation& pos)
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
	bool evt::cCalculateVisibility::Run(cECS::cEntityWithData ed)
	{
		auto vis = ed->second.Component<rl::cmp::cVisibility>();
		auto loc = ed->second.Component<rl::cmp::cLocation>();
		vis->mVisible.clear();
		auto& curexpl = vis->mExplored[loc->mLevelId];
		auto& curvis = vis->mVisible[loc->mLevelId];

		static rlut::cFovLookup<rlut::cFovRsc> fovlut = rlut::cFovLookup<rlut::cFovRsc>();


		auto world = mainecs()->TagusToEntities("World")->second->second.Component<rl::cmp::cWorldData>();
		auto lvl = world->mLevelMap[loc->mLevelId]->second.Component<rl::cmp::cLevelData>();

		// prepare the fov output data
		curexpl.Resize(lvl->mLayout.BgEntities().Width(), lvl->mLayout.BgEntities().Height(), false);
		curvis.Resize(lvl->mLayout.BgEntities().Width(), lvl->mLayout.BgEntities().Height(), false);
		auto onvis = [&](const glm::ivec2& pt, float b) {curexpl(pt) = true; curvis(pt) = true; vis->mVisibleSet.insert(pt); };

		// TODO: this is a parameter of... ? probably here in visibility
		fovlut.Get(4).Calc(loc->mPos,lvl->mLayout.Obstacles(),onvis);

		return true;
	}

	template<>
	bool evt::cCreateLevel::Run(cECS::cEntityWithData ed)
	{
		// TODO: flesh out. Currently store the id -> level association
		auto level = ed->second.Component<rl::cmp::cLevelData>();
		auto world = mainecs()->TagusToEntities("World");
		world->second->second.Component<rl::cmp::cWorldData>()->mLevelMap[level->mId] = ed;
		return true;
	}

}