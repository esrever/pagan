#include "UpdateLayout.h"

#include <rl/event/events.h>
#include <rl/components/components.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			bool cUpdateLayout::operator()(ecs::cEntityWithData& ed, cmp::cLocation * zOldLoc)
			{
				auto world = mainecs()->TagusToEntities("World")->second->second.Component<ecs::cmp::cWorldData>();
				auto newloc = ed->second.Component<cmp::cLocation>();
				auto oldlay = world->mLevelMap[zOldLoc->mLevelId]->second.Component<cmp::cLevelData>()->mLayout;
				auto newlay = world->mLevelMap[newloc->mLevelId]->second.Component<cmp::cLevelData>()->mLayout;

				// is entity on this level? if not, remove, else move there
				// is entity on its level? if not, add, else move there

				return true;
			}
		}
	}
}