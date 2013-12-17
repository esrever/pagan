#include "EntityGlobals.h"

#include <ecs/EntityMgr.h>
#include <ecs/ecs.h>

namespace pgn
{
	cEntityGlobals::cEntityGlobals(cEntityWithData zDef)
		:mPC(zDef)
		, mWorld(zDef)
		, mMapWindow(zDef)
		, mStatusWindow(zDef)
	{}
}