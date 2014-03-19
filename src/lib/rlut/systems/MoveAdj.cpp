#include "MoveAdj.h"

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			bool cMoveAdj::operator()(ecs::cEntityWithData&, const glm::ivec2& dir)
			{
				// get some components
				auto loc = ed->second.Component<rl::cmp::cLocation>();
				auto world = mainecs()->TagusToEntities("World")->second->second.Component<rl::cmp::cWorldData>();
				auto lvl = world->mLevelMap[loc->mLevelId]->second.Component<rl::cmp::cLevelData>();

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
					return true;
				}
				return false;
			}
		}
	}
}