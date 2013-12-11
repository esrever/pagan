#pragma once

#include <string>
#include <glm/glm.hpp>
#include <ecs/EntityData.h>


namespace pgn
{
	namespace evt
	{
		enum eRL
		{
			LEVEL_CREATED = eBasicECS::num,
			LEVEL_DESTROY,
			LEVEL_LOADED,
			LEVEL_UNLOAD,
			EXIT_APPLICATION,
			LOG,
			ACTION_IDLE,
			ACTION_MOVE_ADJ,
			ACTION_DOOR_OPEN,
			ACTION_DOOR_CLOSE,
			DOOR_OPENED,
			DOOR_CLOSED,
			TILE_IN_LEVEL_CHANGED
		};
		

		typedef cEvent<size_t(eRL::LEVEL_CREATED), cEntityWithData> cLevelCreated;
		typedef cEvent<size_t(eRL::LEVEL_DESTROY), cEntityWithData> cLevelDestroy;
		typedef cEvent<size_t(eRL::LEVEL_LOADED), cEntityWithData> cLevelLoaded;
		typedef cEvent<size_t(eRL::LEVEL_UNLOAD), cEntityWithData> cLevelUnload;
		typedef cEvent<size_t(eRL::EXIT_APPLICATION), void> cExitApplication;
		typedef cEvent<size_t(eRL::LOG), const std::string&,const std::string&> cLog;
		typedef cEvent<size_t(eRL::ACTION_IDLE), cEntityWithData> cActionIdle;
		typedef cEvent<size_t(eRL::ACTION_MOVE_ADJ), cEntityWithData,const glm::ivec2&> cActionMoveAdj;
		typedef cEvent<size_t(eRL::ACTION_DOOR_OPEN), cEntityWithData,cEntityWithData> cActionDoorOpen;
		typedef cEvent<size_t(eRL::ACTION_DOOR_CLOSE), cEntityWithData,cEntityWithData> cActionDoorClose;
		typedef cEvent<size_t(eRL::DOOR_OPENED), cEntityWithData> cDoorOpened;
		typedef cEvent<size_t(eRL::DOOR_CLOSED), cEntityWithData> cDoorClosed;
		typedef cEvent<size_t(eRL::TILE_IN_LEVEL_CHANGED), cEntityWithData> cTileInLevelChanged;
	}
}