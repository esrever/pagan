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
		if(evt::cMoveAdj::Run(ped, dir))
			return evt::cCalculateVisibility::Run(ped);
	}

	template<>
	bool evt::cPlayerAppear::Run(const rl::cmp::cLocation& pos)
	{
		auto ped = mainecs()->TagusToEntities("Player")->second;
		if (evt::cAppear::Run(ped, pos))
			return evt::cCalculateVisibility::Run(ped);
		else
			return false;
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
			ptr->mPos = newpos;
			lvl->mLayout.SetActor(ed);
			return true;
		}
		return false;
	}

	template<>
	bool evt::cAppear::Run(cECS::cEntityWithData ed, const rl::cmp::cLocation& pos)
	{
		auto ptr = ed->second.Component<rl::cmp::cLocation>();
		*ptr = pos;
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


		// TODO: get the level properly
		auto lvl = mainecs()->TagusToEntities("CurrentLevel")->second->second.Component<rl::cmp::cLevelData>();

		// TODO: resize only if necessary
		curexpl.Resize(lvl->mLayout.BgEntities().Width(), lvl->mLayout.BgEntities().Height(), false);
		curvis.Resize(lvl->mLayout.BgEntities().Width(), lvl->mLayout.BgEntities().Height(), false);
		auto onvis = [&](const glm::ivec2& pt, float b) {curexpl(pt) = true; curvis(pt) = true; vis->mVisibleSet.insert(pt); };

		// TODO: visibility map easily obtainable by layout
		cArray2D<bool> vismap(lvl->mLayout.BgEntities().Width(), lvl->mLayout.BgEntities().Height());
		lvl->mLayout.BgEntities().View().VisitRext([&](size_t x, size_t y, const cECS::cArchetypeWithDataConst& ed){ vismap(x, y) = ed->second.Component<rl::cmp::cMoveCost>()->mMoveCost != std::numeric_limits<float>::max(); });

		fovlut.Get(10).Calc(loc->mPos,vismap,onvis);

		return true;
	}
}