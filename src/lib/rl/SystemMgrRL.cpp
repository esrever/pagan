#pragma once

#include "SystemMgrRL.h"
#include "systems/systems.h"

namespace pgn
{
	cSystemMgrRL::cSystemMgrRL()
	:mOnExitApplication(Simple::slot(this, &cSystemMgrRL::OnExitApplication))
	{}
	//-------------------------------------------------
	void cSystemMgrRL::RegisterSystemTypes()
	{
		RegisterAllSystems(*this);
	}

	//-------------------------------------------------
	bool cSystemMgrRL::UpdateFrame()
	{
		/* TODO: 
			TimeKeeperSystem
			ActionMappingSystem
			GameTurnSystem
			AsciiRenderSystem
		*/
		mLoop = true;
		for(auto it : mSystems)
			it.second->Process();
		return mLoop;
	}


	void cSystemMgrRL::OnExitApplication()
	{
		mLoop=false;
	}
}