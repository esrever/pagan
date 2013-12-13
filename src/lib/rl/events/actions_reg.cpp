//This should be automatically generated from the header file.
//Scan in namespace evt{} all void XXX() prototypes

#include "actions.h"
#include "events.h" // add to py

#include <ecs/ActionMgr.h>

namespace pgn
{
	void RegisterAllActions(cActionMgr& zMgr)
	{
		zMgr.AddAction("PlayerIdle", &evt::PlayerIdle);
		zMgr.AddAction("PlayerMoveN", &evt::PlayerMoveN);
		zMgr.AddAction("PlayerMoveS", &evt::PlayerMoveS);
		zMgr.AddAction("PlayerMoveE", &evt::PlayerMoveE);
		zMgr.AddAction("PlayerMoveW", &evt::PlayerMoveW);
		zMgr.AddAction("PlayerMoveNE", &evt::PlayerMoveNE);
		zMgr.AddAction("PlayerMoveSE", &evt::PlayerMoveSE);
		zMgr.AddAction("PlayerMoveNW", &evt::PlayerMoveNW);
		zMgr.AddAction("PlayerMoveSW", &evt::PlayerMoveSW);
		zMgr.AddAction("AppQuit", &evt::AppQuit);
		zMgr.AddAction("AppHelp", &evt::AppHelp);

		const auto& ab = cActionBindings::Get(); // add to py
		for (auto x : ab)
		{
			zMgr.AddAction(x.first, x.second);
		}
	}
}