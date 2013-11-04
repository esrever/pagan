#include "actions.h"
#include "ecs/ActionMgr.h"

#include "IdleAction.h"
#include "MoveEAction.h"
#include "MoveNAction.h"
#include "MoveNEAction.h"
#include "MoveNWAction.h"
#include "MoveSAction.h"
#include "MoveSEAction.h"
#include "MoveSWAction.h"
#include "MoveWAction.h"
#include "QuitProgramAction.h"

namespace pgn
{
    void RegisterAllActions(cActionMgr& zMgr) 
	{
		zMgr.AddAction("Idle",&pgn::action::Idle);
		zMgr.AddAction("MoveE",&pgn::action::MoveE);
		zMgr.AddAction("MoveN",&pgn::action::MoveN);
		zMgr.AddAction("MoveNE",&pgn::action::MoveNE);
		zMgr.AddAction("MoveNW",&pgn::action::MoveNW);
		zMgr.AddAction("MoveS",&pgn::action::MoveS);
		zMgr.AddAction("MoveSE",&pgn::action::MoveSE);
		zMgr.AddAction("MoveSW",&pgn::action::MoveSW);
		zMgr.AddAction("MoveW",&pgn::action::MoveW);
		zMgr.AddAction("QuitProgram",&pgn::action::QuitProgram);
	}
}
