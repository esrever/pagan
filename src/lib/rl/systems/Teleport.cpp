#include "Teleport.h"

#include <rl/event/events.h>
#include <rl/components/components.h>
#include <rl/systems/UpdateLayout.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			bool cTeleport::operator()(ecs::cEntityWithData& ed, const ecs::cmp::cLocation& loc, float tu)
			{
				auto world = mainecs()->TagusToEntities("World")->second->second.Component<ecs::cmp::cWorldData>();
				auto edloc = ed->second.Component<cmp::cLocation>();
				auto tgtlvl = world->mLevelMap[loc.mLevelId]->second.Component<cmp::cLevelData>();

				// TODO: use movecosts!
				// Map checks with obstacle map!
				if (!tgtlvl->mLayout.Bg().Cells().InRange(loc.mPos))
					return false;
				if (tgtlvl->mLayout.Obstacles()(loc.mPos))
					return false;
				
				// Success!

				auto ped = mainecs()->TagusToEntities("Player")->second;
				// Set current level if applicable
				if (ed == ped)
				{
					if (edloc->mLevelId != loc.mLevelId)
						mainecs()->Tagu("CurrentLevel", world->mLevelMap[loc.mLevelId]);
				}

				// set the location & update layout
				auto oldloc = *edloc;
				*edloc = loc;
				mainecs()->System<cUpdateLayout>()(ed, &oldloc);

				// emit event
				evt::cLocationChanged::Sig().emit(ed);

				// ... and turn hint
				evt::cActorAction::Sig().emit(tu);

				return true;
			}
		}
	}
}