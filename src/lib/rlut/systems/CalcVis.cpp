#include "CalcVis.h"

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			// TODO: also execute by listening for entities that successfully update their location and have a visibility component!
			// So, grab entities that have BOTH location AND visibility. 2nd should suffice, as it implies the first
			bool cCalcVis::operator()(ecs::cEntityWithData& ed)
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
				fovlut.Get(4).Calc(loc->mPos, lvl->mLayout.Obstacles(), onvis);

				return true;
			}
		}
	}
}