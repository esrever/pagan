#pragma once

#include <string>

#include <glm/glm.hpp>

#include <ecs/ecs_config.h>
#include <ecs/Entity.h>
#include <ecs/EntityMgr.h>

namespace pgn
{
	namespace cmp
	{

	}

	namespace evt
	{
		enum eRL
		{
			TILE_IN_LEVEL_CHANGED = eBasicECS::num,
			LEVEL_CREATED,
			LEVEL_DESTROY,
			EXIT_APPLICATION,
			LOG,
			ACTION_IDLE,
			ACTION_MOVE_ADJ,
		};

		typedef cEvent<size_t(eRL::TILE_IN_LEVEL_CHANGED), cEntityWithData> cTileInLevelChanged;
		typedef cEvent<size_t(eRL::LEVEL_CREATED), cEntityWithData> cLevelCreated;
		typedef cEvent<size_t(eRL::LEVEL_DESTROY), cEntityWithData> cLevelDestroy;
		typedef cEvent<size_t(eRL::EXIT_APPLICATION)> cExitApplication;
		typedef cEvent<size_t(eRL::LOG), std::string> cLog;
		
		typedef cEvent<size_t(eRL::ACTION_IDLE), cEntity> cActionIdle;
		typedef cEvent<size_t(eRL::ACTION_MOVE_ADJ), cEntity, const glm::ivec2&> cActionMoveAdj;
	}
}