#include "actions.h"
#include "ecs/ActionMgr.h

#include "MoveEAction.h"
#include "MoveNAction.h"
#include "MoveNEAction.h"
#include "MoveNWAction.h"
#include "MoveSAction.h"
#include "MoveSEAction.h"
#include "MoveSWAction.h"
#include "MoveWAction.h"

namespace pgn
{
    void RegisterAllActions(cActionsMgr& zMgr) 
	{
		zMgr.AddAction("MoveEAction",&pgn::action::MoveE);
		zMgr.AddAction("MoveNAction",&pgn::action::MoveN);
		zMgr.AddAction("MoveNEAction",&pgn::action::MoveNE);
		zMgr.AddAction("MoveNWAction",&pgn::action::MoveNW);
		zMgr.AddAction("MoveSAction",&pgn::action::MoveS);
		zMgr.AddAction("MoveSEAction",&pgn::action::MoveSE);
		zMgr.AddAction("MoveSWAction",&pgn::action::MoveSW);
		zMgr.AddAction("MoveWAction",&pgn::action::MoveW);
	}
}
