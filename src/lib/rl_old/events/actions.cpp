#include "actions.h"
#include "events.h"
#include "ecs/ecs.h"
#include "ecs/ActionMgr.h"

namespace pgn
{
	namespace evt
	{
		//-----------------------------------------------------------------
		void PlayerIdle()
		{
			cActionActionIdle::RunEvent(ECS.mEntityMgr->Globals().mPC);
		}

		//-----------------------------------------------------------------
		void PlayerMoveN()
		{
			cActionActionMoveAdj::RunEvent(ECS.mEntityMgr->Globals().mPC, glm::ivec2(0, 1));
		}

		//-----------------------------------------------------------------
		void PlayerMoveS()
		{
			cActionActionMoveAdj::RunEvent(ECS.mEntityMgr->Globals().mPC, glm::ivec2(0, -1));
		}

		//-----------------------------------------------------------------
		void PlayerMoveE()
		{
			cActionActionMoveAdj::RunEvent(ECS.mEntityMgr->Globals().mPC, glm::ivec2(1, 0));
		}

		//-----------------------------------------------------------------
		void PlayerMoveW()
		{
			cActionActionMoveAdj::RunEvent(ECS.mEntityMgr->Globals().mPC, glm::ivec2(-1, 0));
		}

		//-----------------------------------------------------------------
		void PlayerMoveNE()
		{
			cActionActionMoveAdj::RunEvent(ECS.mEntityMgr->Globals().mPC, glm::ivec2(1, 1));
		}

		//-----------------------------------------------------------------
		void PlayerMoveSE()
		{
			cActionActionMoveAdj::RunEvent(ECS.mEntityMgr->Globals().mPC, glm::ivec2(1, -1));
		}

		//-----------------------------------------------------------------
		void PlayerMoveNW()
		{
			cActionActionMoveAdj::RunEvent(ECS.mEntityMgr->Globals().mPC, glm::ivec2(-1, 1));
		}

		//-----------------------------------------------------------------
		void PlayerMoveSW()
		{
			cActionActionMoveAdj::RunEvent(ECS.mEntityMgr->Globals().mPC, glm::ivec2(-1, -1));
		}

		//-----------------------------------------------------------------
		void AppQuit()
		{
			cActionExitApplication::RunEvent();
		}

		//-----------------------------------------------------------------
		void AppHelp()
		{
			assert(false);
		}
	}
}
