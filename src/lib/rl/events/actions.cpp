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
			cActionIdle::mSig.emit(ECS.mEntityMgr->Globals().mPC);
		}

		//-----------------------------------------------------------------
		void PlayerMoveN()
		{
			cActionMoveAdj::mSig.emit(ECS.mEntityMgr->Globals().mPC, glm::ivec2(0, 1));
		}

		//-----------------------------------------------------------------
		void PlayerMoveS()
		{
			cActionMoveAdj::mSig.emit(ECS.mEntityMgr->Globals().mPC, glm::ivec2(0, -1));
		}

		//-----------------------------------------------------------------
		void PlayerMoveE()
		{
			cActionMoveAdj::mSig.emit(ECS.mEntityMgr->Globals().mPC, glm::ivec2(1, 0));
		}

		//-----------------------------------------------------------------
		void PlayerMoveW()
		{
			cActionMoveAdj::mSig.emit(ECS.mEntityMgr->Globals().mPC, glm::ivec2(-1, 0));
		}

		//-----------------------------------------------------------------
		void PlayerMoveNE()
		{
			cActionMoveAdj::mSig.emit(ECS.mEntityMgr->Globals().mPC, glm::ivec2(1, 1));
		}

		//-----------------------------------------------------------------
		void PlayerMoveSE()
		{
			cActionMoveAdj::mSig.emit(ECS.mEntityMgr->Globals().mPC, glm::ivec2(1, -1));
		}

		//-----------------------------------------------------------------
		void PlayerMoveNW()
		{
			cActionMoveAdj::mSig.emit(ECS.mEntityMgr->Globals().mPC, glm::ivec2(-1, 1));
		}

		//-----------------------------------------------------------------
		void PlayerMoveSW()
		{
			cActionMoveAdj::mSig.emit(ECS.mEntityMgr->Globals().mPC, glm::ivec2(-1, -1));
		}

		//-----------------------------------------------------------------
		void AppQuit()
		{
			cExitApplication::mSig.emit();
		}

		//-----------------------------------------------------------------
		void AppHelp()
		{
			assert(false);
		}
	}
}
