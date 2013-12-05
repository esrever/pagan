#include "move.h"

#include <ecs/ecs.h>
#include <rl/events/events.h>

namespace pgn
{
	namespace evt
	{
		void ActionMove(cEntity e, const glm::ivec2& loc)
		{
			/*
				For now, just log that this happened. Later, need to also get the level of the entity, the map, and
				 do the proper movement
			*/

			int loc1 = (loc.x + 1) + (loc.y + 1) * 3;
			const char * locnames[] = { "SW", "S", "SE", "W", "nowhere", "E", "NW", "N", "NE" };

			auto ec = ECS.mEntityMgr->GetEntityData(e);

			auto msg = boost::str(boost::format("%ss moved %s")%ec.mName.c_str()%locnames[loc1]);
			evt::cLog::mSig.emit("game",msg);
		}
	}
}