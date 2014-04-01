#include "MoveAdj.h"

#include <core/math/norm.h>

#include <rl/event/events.h>
#include <rl/components/components.h>
#include <rl/systems/UpdateLayout.h>
#include <rl/systems/StatsProc.h>

static const char * dirstrings_long[] = { "southwest", "south", "southeast", "west", "nowhere", "east", "northwest", "north", "northeast" };

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			bool cMoveAdj::operator()(ecs::cEntityWithData& ed, const glm::ivec2& dir)
			{
				// if entity didn't move anywhere, still emit an action event
				if (dir == glm::ivec2(0, 0))
				{
					if (ed == mainecs()->TagusToEntities("Player")->second)
					{
						mainapp()->GameLog().Inf(pgn::format("%s moves %s", ed->second.mName.c_str(), dirstrings_long[dir.x + 1 + 3 * (dir.y + 1)]));
					}
					evt::cActorAction::Sig().emit(1.0f);
					
					return false;
				}

				// get some components
				auto loc = ed->second.Component<cmp::cLocation>();

				auto world = mainecs()->TagusToEntities("World")->second->second.Component<cmp::cWorldData>();
				auto lvl = world->mLevelMap[loc->mLevelId]->second.Component<cmp::cLevelData>();

				// calc new position
				auto newpos = loc->mPos + dir;

				bool is_player = ed == mainecs()->TagusToEntities("Player")->second;

				// TODO: use movecosts!
				// Map checks with obstacle map!
				if (!lvl->mLayout.Bg().Cells().InRange(newpos))
					return false;
				if (!lvl->mLayout.Obstacles()(newpos))
				{
					// no collisions; set new position and update map
					auto oldloc = *loc;
					loc->mPos = newpos;
					mainecs()->System<cUpdateLayout>()(ed, &oldloc);

					evt::cLocationChanged::Sig().emit(ed);
					
					// Player-specific log!
					if (is_player)
					{
						mainapp()->GameLog().Inf(pgn::format("%s moves %s", ed->second.mName.c_str(), dirstrings_long[dir.x + 1 + 3 * (dir.y + 1)]));
						// ... and turn hint
					}
					evt::cActorAction::Sig().emit(norm_2(dir));
					return true;
				}
				else
				{
					// bump-attack
					if (is_player && (!lvl->mLayout.Actors().Cells().View().IsClear(newpos.x, newpos.y)))
					{
						auto nb = lvl->mLayout.Actors().Cells()(newpos);
						mainecs()->System<cStatsProc>().MeleeAttack(ed, nb);
					}
				}
				return false;
			}
		}
	}
}