#include "CalcVis.h"

#include <rl/components/components.h>
#include <rl/fov/FovLookup.h>
#include <rl/fov/fov_rsc.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			//-------------------------------------------------------------------------
			cCalcVis::cCalcVis() :INIT_EVT_MEMBER(cCalcVis, LocationChanged){}

			//-------------------------------------------------------------------------
			void cCalcVis::OnLocationChanged(const ecs::cEntityWithData& ed)
			{
				if (!Active()) return;
				// TODO: more formal
				if (!ed->second.Component<cmp::cVisibility>()) return;
				(*this)(const_cast<ecs::cEntityWithData&>(ed));
			}

			//-------------------------------------------------------------------------
			bool cCalcVis::operator()(ecs::cEntityWithData& ed)
			{
				auto vis = ed->second.Component<cmp::cVisibility>();
				auto loc = ed->second.Component<cmp::cLocation>();
				auto& curexpl = vis->mExplored[loc->mLevelId];
				auto& curvis = vis->mVisible;

				static rl::cFovLookup<rl::cFovRsc> fovlut = rl::cFovLookup<rl::cFovRsc>();


				auto world = mainecs()->TagusToEntities("World")->second->second.Component<cmp::cWorldData>();
				auto lvl = world->mLevelMap[loc->mLevelId]->second.Component<cmp::cLevelData>();

				// prepare the fov output data
				curexpl.Resize(lvl->mLayout.Dims().x, lvl->mLayout.Dims().y, false);
				curvis.Resize(lvl->mLayout.Dims().x, lvl->mLayout.Dims().y, false);
				auto onvis = [&](const glm::ivec2& pt, float b) {curexpl(pt) = true; curvis(pt) = true; vis->mVisibleSet.insert(pt); };

				// TODO: this is a parameter of... ? probably here in visibility
				fovlut.Get(4).Calc(loc->mPos, lvl->mLayout.Obstacles(), onvis);

				return true;
			}
		}
	}
}