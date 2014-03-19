#include "MoveAdj.h"

#include <rlut/event/events.h>
#include <rlut/components/components.h>

static const char * dirstrings_long[] = { "southwest", "south", "southeast", "west", "nowhere", "east", "northwest", "north", "northeast" };

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			bool cMoveAdj::operator()(ecs::cEntityWithData& ed, const glm::ivec2& dir)
			{
				// get some components
				auto loc = ed->second.Component<cmp::cLocation>();
				auto world = mainecs()->TagusToEntities("World")->second->second.Component<cmp::cWorldData>();
				auto lvl = world->mLevelMap[loc->mLevelId]->second.Component<cmp::cLevelData>();

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

					evt::cLocationChanged::Sig().emit(ed);
					
					// Player-specific log!
					if (ed == mainecs()->TagusToEntities("Player")->second)
						mainapp()->GameLog().Inf(pgn::format("%s moves %s", ed->second.mName.c_str(), dirstrings_long[dir.x + 1 + 3 * (dir.y + 1)]));
					return true;
				}
				return false;
			}
		}
	}
}