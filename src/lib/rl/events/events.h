#pragma once

#include <string>
#include <glm/glm.hpp>
#include <ecs/EntityData.h>

namespace oxygine
{
	struct cKeyState;
}

namespace pgn
{
	namespace evt
	{
		enum eRL
		{
			LEVEL_CREATE = eBasicECS::num,
			LEVEL_DESTROY,
			LEVEL_LOAD,
			LEVEL_UNLOAD,
			LEVEL_ENTER,
			LEVEL_LEAVE,
			EXIT_APPLICATION,
			LOG,
			ACTION_IDLE,
			ACTION_MOVE_ADJ,
			DOOR_OPEN,
			DOOR_CLOSE,
            KEY_STATE,
			LOCATION_CHANGE
		};		

		typedef cEvent<size_t(eRL::LEVEL_CREATE), cEntityWithData> cLevelCreate;
		typedef cEvent<size_t(eRL::LEVEL_DESTROY), cEntityWithData> cLevelDestroy;
		typedef cEvent<size_t(eRL::LEVEL_LOAD), cEntityWithData> cLevelLoad;
		typedef cEvent<size_t(eRL::LEVEL_UNLOAD), cEntityWithData> cLevelUnload;
		typedef cEvent<size_t(eRL::LEVEL_ENTER), cEntityWithData, cEntityWithData> cLevelEnter;
		typedef cEvent<size_t(eRL::LEVEL_LEAVE), cEntityWithData, cEntityWithData> cLevelLeave;
		typedef cEvent<size_t(eRL::EXIT_APPLICATION), void> cExitApplication;
		typedef cEvent<size_t(eRL::LOG), const std::string&,const std::string&> cLog;
		typedef cEvent<size_t(eRL::ACTION_IDLE), cEntityWithData> cActionIdle;
		typedef cEvent<size_t(eRL::ACTION_MOVE_ADJ), cEntityWithData,const glm::ivec2&> cActionMoveAdj;
		typedef cEvent<size_t(eRL::DOOR_OPEN), cEntityWithData,cEntityWithData> cDoorOpen;
		typedef cEvent<size_t(eRL::DOOR_CLOSE), cEntityWithData,cEntityWithData> cDoorClose;
		typedef cEvent<size_t(eRL::KEY_STATE), const int, const oxygine::cKeyState&> cKeyState;
		typedef cEvent<size_t(eRL::LOCATION_CHANGE), cEntityWithData, cEntityWithData, const glm::ivec2&> cLocationChange;
	}

	typedef cAction<size_t(evt::eRL::LEVEL_CREATE), cEntityWithData> cActionLevelCreate;
	typedef cAction<size_t(evt::eRL::LEVEL_DESTROY), cEntityWithData> cActionLevelDestroy;
	typedef cAction<size_t(evt::eRL::LEVEL_LOAD), cEntityWithData> cActionLevelLoad;
	typedef cAction<size_t(evt::eRL::LEVEL_UNLOAD), cEntityWithData> cActionLevelUnload;
	typedef cAction<size_t(evt::eRL::LEVEL_ENTER), cEntityWithData, cEntityWithData> cActionLevelEnter;
	typedef cAction<size_t(evt::eRL::LEVEL_LEAVE), cEntityWithData, cEntityWithData> cActionLevelLeave;
	typedef cAction<size_t(evt::eRL::EXIT_APPLICATION)> cActionExitApplication;
	typedef cAction<size_t(evt::eRL::LOG), const std::string&, const std::string&> cActionLog;
	typedef cAction<size_t(evt::eRL::ACTION_IDLE), cEntityWithData> cActionActionIdle;
	typedef cAction<size_t(evt::eRL::ACTION_MOVE_ADJ), cEntityWithData, const glm::ivec2&> cActionActionMoveAdj;
	typedef cAction<size_t(evt::eRL::DOOR_OPEN), cEntityWithData, cEntityWithData> cActionDoorOpen;
	typedef cAction<size_t(evt::eRL::DOOR_CLOSE), cEntityWithData, cEntityWithData> cActionDoorClose;
	typedef cAction<size_t(evt::eRL::KEY_STATE), const int, const oxygine::cKeyState&> cActionKeyState;
	typedef cAction<size_t(evt::eRL::LOCATION_CHANGE), cEntityWithData, cEntityWithData, const glm::ivec2&> cActionLocationChange;
}