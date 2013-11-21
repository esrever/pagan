#pragma once

#include <ecs/ecs_config.h>
#include <ecs/Entity.h>
#include <ecs/EntityMgr.h>

namespace pgn
{
	namespace evt
	{
		enum eRL
		{
			TILE_IN_LEVEL_CHANGED = eBasicECS::num,
			LEVEL_CREATED,
			LEVEL_DESTROY,
			EXIT_APPLICATION,
		};

		typedef cEvent<size_t(eRL::TILE_IN_LEVEL_CHANGED), cEntityWithData> cTileInLevelChanged;
		typedef cEvent<size_t(eRL::LEVEL_CREATED), cEntityWithData> cLevelCreated;
		typedef cEvent<size_t(eRL::LEVEL_DESTROY), cEntityWithData> cLevelDestroy;
		typedef cEvent<size_t(eRL::EXIT_APPLICATION)> cExitApplication;
	}
}