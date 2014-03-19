#include "Teleport.h"

#include <rlut/event/events.h>
#include <rlut/components/components.h>

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

				// success!
				// TODO: remove from old level
				//auto srclvl = world->mLevelMap[edloc->mLevelId]->second.Component<cmp::cLevelData>();
				//srclvl->mLayout.RemoveActor(ed);

				if (ed == mainecs()->TagusToEntities("Player")->second)
				{
					if (edloc->mLevelId != loc.mLevelId)
						mainecs()->Tagu("CurrentLevel", world->mLevelMap[loc.mLevelId]);
				}
				*edloc = loc;
				tgtlvl->mLayout.SetActor(ed);
				evt::cLocationChanged::Sig().emit(ed);
				return true;
			}
		}
	}
}