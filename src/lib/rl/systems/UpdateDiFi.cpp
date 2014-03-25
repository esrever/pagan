#include "UpdateDiFi.h"

#include <rl/components/components.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			//-------------------------------------------------------------------------
			cUpdateDiFi::cUpdateDiFi() :INIT_EVT_MEMBER(cUpdateDiFi, LocationChanged){}

			//-------------------------------------------------------------------------
			void cUpdateDiFi::OnLocationChanged(const ecs::cEntityWithData& ed)
			{
				if (!Active()) return;
				// TODO: more formal
				if (!ed->second.Component<cmp::cMapDiFi>()) return;
				(*this)(const_cast<ecs::cEntityWithData&>(ed));
			}

			//-------------------------------------------------------------------------
			bool cUpdateDiFi::operator()(ecs::cEntityWithData& ed)
			{
				auto& difi = ed->second.Component<cmp::cMapDiFi>()->mValue;
				auto loc = ed->second.Component<cmp::cLocation>();

				auto world = mainecs()->TagusToEntities("World")->second->second.Component<ecs::cmp::cWorldData>();
				auto it_lvl = world->mLevelMap.find(loc->mLevelId);
				if (it_lvl == world->mLevelMap.end()) return false;
				auto& lay = it_lvl->second->second.Component<cmp::cLevelData>()->mLayout;

				// Using static move costs!
				auto mfunc = [&](const glm::ivec2& p0, const glm::ivec2& p1){
					if (lay.StaticMoveCosts().InRange(p1))
						return lay.StaticMoveCosts()(p1);
					else
						return std::numeric_limits<float>::max();
				};

				// We feed the corner and the goal in world coordinate
				const auto& goal = loc->mPos;
				glm::ivec2 r(difi.Data().Width() / 2, difi.Data().Height() / 2);
				auto corner = goal - r;
				if (corner.x < 0) corner.x = 0;
				if (corner.y < 0) corner.y = 0;
				difi.Generate(mfunc, corner, goal);

				return true;
			}
		}
	}
}