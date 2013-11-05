#pragma once

#include <ecs/ecs_config.h>
#include <ecs/Entity.h>
#include <ecs/EntityMgr.h>
#include <rl/components/Location.h>

namespace pgn
{
	enum eRLEvents
	{
		TILE_IN_LEVEL_CHANGED= eBasicECSEvent::num,
		LEVEL_CREATED,
		LEVEL_DESTROY,
		MOVE_TO_ADJACENT,
		EXIT_APPLICATION
	};

	typedef cEvent<size_t(eRLEvents::TILE_IN_LEVEL_CHANGED), cEntityWithComponents> cTileInLevelChangedEvent;
	typedef cEvent<size_t(eRLEvents::LEVEL_CREATED), cEntityWithComponents> cLevelCreatedEvent;
	typedef cEvent<size_t(eRLEvents::LEVEL_DESTROY), cEntityWithComponents> cLevelDestroyEvent;
	typedef cEvent<size_t(eRLEvents::MOVE_TO_ADJACENT), cEntityWithComponents, const cLocation&> cMoveToAdjacentEvent;
	typedef cEvent<size_t(eRLEvents::EXIT_APPLICATION)> cExitApplicationEvent;
}