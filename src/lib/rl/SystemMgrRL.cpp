#pragma once

#include "SystemMgrRL.h"
#include "systems/systems.h"

namespace pgn
{
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
		for(auto it : mSystems)
			it.second->Process();
		return true;
	}
}