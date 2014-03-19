#include "Teleport.h"

#include <rl/event/events.h>
#include <rl/components/components.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			bool cTeleport::operator()(ecs::cEntityWithData& ed, const ecs::cmp::cLocation& loc)
			{
				auto world = mainecs()->TagusToEntities("World")->second->second.Component<ecs::cmp::cWorldData>();
				auto edloc = ed->second.Component<cmp::cLocation>();
				auto tgtlvl = world->mLevelMap[loc.mLevelId]->second.Component<cmp::cLevelData>();

				// Map checks with obstacle map!
				if (!tgtlvl->mLayout.BgEntities().InRange(loc.mPos))
					return false;
				if (tgtlvl->mLayout.Obstacles()(loc.mPos))
					return false;
				
				// Success!
				
				// remove from old level
				auto it_srclvl = world->mLevelMap.find(edloc->mLevelId);
				if (it_srclvl != world->mLevelMap.end())
				{
					auto srclvl = it_srclvl->second->second.Component<cmp::cLevelData>();
					srclvl->mLayout.RemoveActor(ed);
				}

				// Set current level if applicable
				if (ed == mainecs()->TagusToEntities("Player")->second)
				{
					if (edloc->mLevelId != loc.mLevelId)
						mainecs()->Tagu("CurrentLevel", world->mLevelMap[loc.mLevelId]);
				}

				// set the location
				*edloc = loc;
				
				// update layout
				tgtlvl->mLayout.SetActor(ed);

				// emit event
				evt::cLocationChanged::Sig().emit(ed);

				return true;
			}
		}
	}
}